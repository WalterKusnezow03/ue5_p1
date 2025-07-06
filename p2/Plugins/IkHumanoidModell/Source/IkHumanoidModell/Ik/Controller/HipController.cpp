#include "HipController.h"
#include "GameCore/DebugHelper.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/SlipContainer.h"

HipController::HipController(){

}

HipController::~HipController(){

}

void HipController::drawLocation(float deltatime){
    if(worldPointer){
        deltatime *= 2.0f;
        FVector locationCurrent = translation.getTranslation();
        DebugHelper::showLineBetween(worldPointer, FVector(0, 0, 0), locationCurrent, FColor::Cyan);
        //DebugHelper::showScreenMessage("location ", locationCurrent);
    }
}

void HipController::setLocation(FVector &other){
    translation.setTranslation(other);

    //find new ground truth when position is changed
    //might falling needed
    FVector ignored;
    projectToGround(ignored);
}

void HipController::setup(UWorld *world){
    worldPointer = world;

    float lengthA = 50.0f;
    float lengthB = 50.0f;
    setupLegLength = lengthA + lengthB;

    //x is forward
    FVector offsetLeft(0, -20.0f, 0.0f);
    legLeft.setupBone(lengthA, lengthB, world, offsetLeft);

    FVector offsetRight = offsetLeft * -1.0f;
    legRight.setupBone(lengthA, lengthB, world, offsetRight);



    //setup forward reach target
    float lengthTotal = std::abs(lengthA) + std::abs(lengthB);

    forwardDefaultLocalLocomotionFrame = FVector(40.0f, 0, -lengthTotal);
    DebugHelper::logMessage("Default Forward Trajectory unprojected", forwardDefaultLocalLocomotionFrame);
    forwardRotatedLocalLocomotionFrame = forwardDefaultLocalLocomotionFrame;


    //debug
    //forwardDefaultLocalLocomotionFrame = FVector(40.0f, 20, -lengthTotal);
    //forwardRotatedLocalLocomotionFrame = forwardDefaultLocalLocomotionFrame;

    buildOnStart();
}

void HipController::buildOnStart(){

    //build bones once
    float deltatime = 1.0f;
    MMatrix root = translation * orientation; //<-- lese richtung --
    legLeft.TickNone(root, deltatime);
    legRight.TickNone(root, deltatime);

    setupForwardInterpolation();
}


bool HipController::groundedByDistance(){
    /*
    check ground distance by default hip location above ground
    */
    float hipAbovegroundZMin = latestGroundTruth.Z + setupLegLength;
    float ownZ = translation.getTranslation().Z;

    bool grounded = ownZ <= hipAbovegroundZMin;

    //reset velocity when grounded 
    //VERY IMPORTANT!
    if(grounded){
        float z = velocity.Z;
        velocity.Z = std::max(z, 0.0f);
    }


    //DEBUG
    grounded = true;

    return grounded;
}

void HipController::Tick(float deltatime){
    //TickLocomotion(deltatime);

    FString message;
    if (groundedByDistance())
    {

        
        if(DEBUG_SLOWTIME){
            float timesSlower = 5.0f;
            deltatime = 1.0f / (timesSlower * 60.0f); // DEBUG
        }
            

        TickHipRotation(deltatime);
        TickLocomotion(deltatime);
        message = TEXT("locomotion");
    }
    else
    {
        TickFalling(deltatime);
        message = TEXT("falling");
    }

    DebugHelper::showScreenMessage(message);
}

void HipController::TickLocomotion(float deltatime){
    UpdateStanceStatus();
    updateInterpolatorLocomotion(deltatime);
    applyLocomotion(deltatime);
    applyForces(deltatime);
}

void HipController::TickFalling(float deltatime){
    applyForceGravity(deltatime);
    applyVelocity(deltatime);
    RebuildLegsEndInPlaceFaceDown(deltatime);
    /*MMatrix transform = translation * orientation; //<-- lese richtung --
    legLeft.TickNone(transform, deltatime);
    legRight.TickNone(transform, deltatime);*/
}

//apply locomotion
void HipController::applyLocomotion(float deltatime){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;
    MMatrix transform = translation * orientation; //<-- lese richtung --
    if(forwardMotion){
        
        attachment.TickForwardKinematic(transform, deltatime);

        /*
        MMatrix orientationCopy = orientation;
        attachment.TickForwardKinematicOutOfReachTarget(
            translation,
            orientationCopy, // könnte temporäre kopie sein
            deltatime
        );*/

        //FString message = legLeftPlaying ? TEXT("leg 1 forward") : TEXT("leg 2 forward ");
        //DebugHelper::showScreenMessage(message);
    }
    else
    {
        attachment.TickBackwardKinematic(
            translation, 
            orientation,
            deltatime
        );

        //FString message = legLeftPlaying ? TEXT("leg 1 backward") : TEXT("leg 2 backward ");
        //DebugHelper::showScreenMessage(message);
    }


    BoneAttachment &other = !legLeftPlaying ? legLeft : legRight;
    //other.TickNone(transform, deltatime);
    other.TickKeepEndInWorldPlace(
        translation,
        orientation,
        deltatime
    );
}



void HipController::UpdateStanceStatus(){
    if(legLeftPlaying){
        if(forwardMotion){
            phaseLeft = ELegPhase::EForward;
            phaseRight = ELegPhase::EEndInPlace;
        }
        else
        {
            phaseLeft = ELegPhase::EBackward;
            phaseRight = ELegPhase::EEndInPlace;
        }
    }else{
        if(forwardMotion){
            phaseRight = ELegPhase::EForward;
            phaseLeft = ELegPhase::EEndInPlace;
        }
        else
        {
            phaseRight = ELegPhase::EBackward;
            phaseLeft = ELegPhase::EEndInPlace;
        }
    }
}

bool HipController::anyBackwardPhase(){
    return phaseLeft == ELegPhase::EBackward || phaseRight == ELegPhase::EBackward;
}

//interpolation update
void HipController::updateInterpolatorLocomotion(float deltatime){
    if(forwardMotion){
        //get world, push to current bone as target
        FVector targetWorld = interpolatorForwardWorld.interpolate(deltatime);
        updateForwardTargetWorld(targetWorld);
        if(interpolatorForwardWorld.hasReachedTarget()){
            forwardMotion = false;
            setupBackwardInterpolation();
            return;
        }

    }else{

        //get local, push as currentTarget
        FVector targetLocal = interpolatorBackwardLocal.interpolate(deltatime);
        updateBackwardTargetLocal(targetLocal);
        if(interpolatorBackwardLocal.hasReachedTarget()){
            forwardMotion = true;
            //switch leg for next setup
            legLeftPlaying = !legLeftPlaying;
            setupForwardInterpolation();
            return;
        }
    }
}

/**
 * update target for bone attachments
 */
//update forward target according to current leg moving
void HipController::updateForwardTargetWorld(FVector &targetWorld){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;

    attachment.setForwardTargetWorld(
        targetWorld,
        translation,
        orientation
    );
}

void HipController::updateBackwardTargetLocal(FVector &targetLocal){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;
    attachment.setBackwardTargetLocal(targetLocal);
}


/**
 * interpolator updates
 */

/// @brief updates the interpolator for backwards kinematic
void HipController::setupBackwardInterpolation(){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;

    FVector localStart = attachment.hipRelativeLocationToEndEffector(
        translation,
        orientation
    );
    FVector localEnd = attachment.defaultExtendedEndToStartLocal();

    float dynamicMotionTime = animationTimeBasedOnCurrentVelocity(localStart, localEnd);
    interpolatorBackwardLocal.setTarget(localStart, localEnd, dynamicMotionTime);



    //PRE CALCULATE D 
    
    //A: end effector current relative to hip
    //B: end effector lift off, when next step touches ground relative to hip -- immer der default aber rückwärts?
    //komischer hack, muss genau definiert werden!

    FVector b = forwardDefaultLocalLocomotionFrame;
    b.X *= -1.0f; //to back when lifting off ground --> is overriden internally
    float velocityDown = velocity.Z;
    attachment.setupSlipDataOnStanceBegin(
        orientation,
        b, // local end on liftoff
        dynamicMotionTime,//motionTime,
        velocityDown, //current velocity downwards to overcome
        bodyMass
    );


    //SETUP ROTATION
    hipRotationInterpolator.overrideTime(dynamicMotionTime);


}

void HipController::setupForwardInterpolation(){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;


    //move trajectory to future with velocity
    //FVector localTrajectory = forwardDefaultLocalLocomotionFrame;

    //CAUTION: NEW: Rotated trajectory copy, reset rotation
    FVector localTrajectory = forwardTrajectory();

    // t motionTime
    //gx = a + t (b-a)
    FVector localTrjectoryRotatedSpace = orientation * localTrajectory;
    localTrjectoryRotatedSpace += motionTime * velocity;
    MMatrix orientationInverse = orientation.transposedRotation();
    localTrajectory = orientationInverse * localTrjectoryRotatedSpace;

    //move trajectory to world space
    FVector worldTrajectory = attachment.inWorldSpace(
        localTrajectory, //forwardDefaultLocalLocomotionFrame,
        translation,
        orientation
    );

    //project frame to ground
    FVector worldTrajectoryProjected = worldTrajectory;
    projectToGround(worldTrajectoryProjected);

    //set as target with end and current Starting point (end effector)
    FVector currentEndEffector = attachment.endEffectorWorldLocation();

    if(false){
        DebugHelper::logMessage("hipcontroller: forward start ", currentEndEffector);
        DebugHelper::logMessage("hipcontroller: forward end ", worldTrajectoryProjected);
    }
    
    DebugHelper::showLineBetween(
        worldPointer, currentEndEffector, currentEndEffector + FVector(0, 0, 30), FColor::Orange, 1.0f
    );
    DebugHelper::showLineBetween(
        worldPointer, currentEndEffector, worldTrajectoryProjected , FColor::Yellow, 1.0f
    );


    
    interpolatorForwardWorld.setTarget(
        currentEndEffector, 
        worldTrajectoryProjected, 
        motionTime
    );

    //interpolatorForwardWorld.setTarget(currentEndEffector, worldTrajectoryProjected, motionTime);
}


void HipController::projectToGround(FVector &worldTarjectory){
    if(!worldPointer){
        return;
    }

    //debug
    bool debugSkip = false;
    if(debugSkip){
        return;
    }

    FVector Start = worldTarjectory + FVector(0, 0, 600);
    FVector End = worldTarjectory + FVector(0, 0, -1000000);


    // Perform the raycast
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.bTraceComplex = false;
    bool bHit = worldPointer->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

    // If the raycast hit something, save the collision point
    if (bHit)
    {
        FVector worldImpact = HitResult.ImpactPoint;
        worldTarjectory = worldImpact;
        latestGroundTruth = worldImpact;
    }
}





/// ---- force section ----
void HipController::applyForces(float deltatime){

    //es ist noch nicht klar ob die slip force an die gravity geknüpft wird,
    //eigentlich ja.
    applyStancePhaseSLIPForce(deltatime);

    //unklar ob es hier bleibt
    applyForceGravity(deltatime);

    //applyForceGravity(deltatime);
    applyVelocity(deltatime);

    //very important
    //rebuild both to update end effectors - is needed in case of backward kinematic.
    RebuildLegsEndInPlace(deltatime);
}


void HipController::applyVelocity(float deltatime){

    //debug
    DebugHelper::showScreenMessage("velocity ", velocity, FColor::Orange);

    applyMaxVelocity();

    //x(t) = x0 + v0t + fällt weg(0.5at^2)?
    FVector x0 = translation.getTranslation();
    FVector xt = x0 + velocity * deltatime;
    validateTransformUpdate(xt);

    DebugHelper::showScreenMessage("x(t) ", xt);
    translation.setTranslation(xt);

}

//for default rebuild
void HipController::RebuildLegsEndInPlace(float deltatime){
    legLeft.TickKeepEndInWorldPlace(
        translation,
        orientation,
        deltatime
    );
    legRight.TickKeepEndInWorldPlace(
        translation,
        orientation,
        deltatime
    );
}

//for gravity apply
void HipController::RebuildLegsEndInPlaceFaceDown(float deltatime){
    legLeft.TickKeepEndInWorldPlaceNegHeightTrajectory(
        translation,
        orientation,
        deltatime
    );
    legRight.TickKeepEndInWorldPlaceNegHeightTrajectory(
        translation,
        orientation,
        deltatime
    );
}




//hack
void HipController::applyMaxVelocity(){
    FVector2D velocity2D(velocity.X, velocity.Y);
    float maxHorizontalVelocity = 200.0f;
    if(velocity2D.Size() > maxHorizontalVelocity){
        velocity2D = velocity2D.GetSafeNormal() * maxHorizontalVelocity;
        velocity.X = velocity2D.X;
        velocity.Y = velocity2D.Y;
    }
}

/// @brief clamps the position update to not fall below ground
/// @param position 
void HipController::validateTransformUpdate(FVector &position){
    float heightUpdate = std::max(position.Z, latestGroundTruth.Z);
    position.Z = heightUpdate;
}

/// @brief returns if left leg is in stance phase
/// @return is in stance phase for slip force
bool HipController::leftInStancePhase(){
    return phaseLeft == ELegPhase::EEndInPlace || phaseLeft == ELegPhase::EBackward;
}

/// @brief returns if left leg is in stance phase
/// @return is in stance phase for slip force
bool HipController::rightInStancePhase(){
    return phaseRight == ELegPhase::EEndInPlace || phaseRight == ELegPhase::EBackward;
}

void HipController::applyStancePhaseSLIPForce(float deltatime){

    //move dir for slip force
    FVector moveDir(1, 0, 0);
    moveDir = orientation * moveDir;


    TArray<BoneAttachment *> attachmentsToEvaluateForce;
    if(leftInStancePhase()){
        attachmentsToEvaluateForce.Add(&legLeft);
    }
    if(rightInStancePhase()){
        attachmentsToEvaluateForce.Add(&legRight);
    }

    FVector acceleration(0, 0, 0);
    for (int i = 0; i < attachmentsToEvaluateForce.Num(); i++)
    {
        BoneAttachment *currentBoneAttachment = attachmentsToEvaluateForce[i];
        if(currentBoneAttachment){
            //if the foot is actually grounded apply force
            SlipContainer &container = currentBoneAttachment->slipData();

            /**
             * was hier noch nicht klar sit ob die kraft auch 
             * in das welt koordintane system gebracht werden muss.
             * 
             * wobei ja start und end effektor in welt kordinaten ausgedrückt sind
             * und somit die federkraft sich auch in der welt befindet
             * 
             * die frage die sich noch stellt ist ob die stance phase so
             * stimmt
             */
            FVector accelerationCurrent = container.accelerationInterpolated(
                deltatime,
                bodyMass,
                moveDir
            );

            acceleration += accelerationCurrent;
        }
    }

    //x(t) = x0 + v0t + 0.5at^2
    //v(t) = v0 + at
    //a(t) = a

    //old simple
    //FVector acceleration = container.acceleration(bodyMass, moveDir);


    DebugHelper::showScreenMessage("acceleration ", acceleration, FColor::Orange);

    velocity += acceleration * deltatime;



    
}

void HipController::applyForceGravity(float deltatime){
    //apply gravity if not below
    //ground 
    //maybe by adding extra distance (but not sure) to keep from ground

    float gravityScale = 1.0f;

    float currentZheight = translation.getTranslation().Z;
    float groundZheight = latestGroundTruth.Z;
    



    
    if(currentZheight > groundZheight){
        FVector acceleration(0, 0, -981 * gravityScale);
        velocity += acceleration * deltatime;
    }

}




float HipController::horizontalVelocity(){
    FVector copy = velocity;
    copy.Z = 0.0f;
    return copy.Size();
}

float HipController::animationTimeBasedOnCurrentVelocity(
    FVector &localStart,
    FVector &localEnd
){
    //v = m / s

    //m = dist ab
    //v = velocity
    //s = animtime 

    //s = m / v

    float m = FVector::Dist(localStart, localEnd);

    float v = horizontalVelocity();

    //1cm s
    if(v < 1.0f){
        return motionTime; //default motion time fallback
    }

    float time = m / v;
    return time;
}





// ----- rotation section experimental ------
FVector HipController::forwardTrajectory(){
    FVector localTrajectory;
    bool newRotatedTrajectory = true;
    if (newRotatedTrajectory)
    {
        localTrajectory = forwardRotatedLocalLocomotionFrame;

        //reset rotation after step
        forwardRotatedLocalLocomotionFrame = forwardDefaultLocalLocomotionFrame;
    }
    else
    {
        localTrajectory = forwardDefaultLocalLocomotionFrame;
    }
    return localTrajectory;
}

void HipController::setupRotationForNextStep(float radian){
    MMatrix addYawMat;
    addYawMat.yawRadAdd(radian);
    forwardRotatedLocalLocomotionFrame = addYawMat * forwardDefaultLocalLocomotionFrame;


    //setup interpolation of rotation
    FRotator current = orientation.extractRotator();
    current.Roll = 0.0f;
    current.Pitch = 0.0f;
    FRotator end = current;
    end.Yaw += MMatrix::radToDegree(radian);

    float timeScaled = 1.0f; //will be overriden on backward interpolation
    hipRotationInterpolator.setTarget(current, end, timeScaled);
    // FRotator TargetInterpolator::interpolateRotationOnly(float DeltaTime)

    rotationSet = true;
}

void HipController::TickHipRotation(float deltatime){
    //debug
    //return;

    if(rotationSet && anyBackwardPhase()){
        if(hipRotationInterpolator.endReached()){
            rotationSet = false;
            return;
        }

        FRotator rTicked = hipRotationInterpolator.interpolate(deltatime);
        MMatrix newRotation(rTicked);
        orientation = rTicked;
        
    }
}
