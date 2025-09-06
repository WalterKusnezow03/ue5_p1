#include "HipController.h"
#include "GameCore/DebugHelper.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/SlipContainer.h"

HipController::HipController(){
    rotationSet = false;
}

HipController::~HipController(){

}

// do not modify 
MMatrix &HipController::getOrientation(){
    return orientation;
}

//do not modify
MMatrix &HipController::getTranslation(){
    return translation;
}

void HipController::SetTranslation(FVector &target){
    translation.setTranslation(target);
}

FVector HipController::GetLocation(){
    return translation.getTranslation();
}

void HipController::SetLocation(FVector &target){

    FVector hipTarget = target;
    hipTarget.Z += setupLegLength;
    translation.setTranslation(hipTarget);

    //find new ground truth when position is changed
    //might falling needed
    FVector ignored;
    projectToGround(ignored);
}

void HipController::drawLocation(float deltatime){
    if(worldPointer){
        deltatime *= 2.0f;
        FVector locationCurrent = translation.getTranslation();
        DebugHelper::showLineBetween(worldPointer, FVector(0, 0, 0), locationCurrent, FColor::Cyan);
        //DebugHelper::showScreenMessage("location ", locationCurrent);
    }
}


void HipController::forceYawAdd(float degree){
    orientation.yawRadAdd(MMatrix::degToRadian(degree));
}

void HipController::setup(UWorld *world){
    worldPointer = world;

    float lengthA = 50.0f;
    float lengthB = 50.0f;
    setupLegLength = lengthA + lengthB;


    //x is forward
    FVector offsetLeft(0, -20.0f, 0.0f);
    legLeft.setupBone(lengthA, lengthB, world, offsetLeft, bodyMass, motionTime);
    legLeft.setAsLeg();

    FVector offsetRight = offsetLeft * -1.0f;
    legRight.setupBone(lengthA, lengthB, world, offsetRight, bodyMass, motionTime);
    legRight.setAsLeg();

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
    float epsilon = 1.3f; //might be adjusted
    float hipAbovegroundZMin = latestGroundTruth.Z + setupLegLength * epsilon;
    float ownZ = translation.getTranslation().Z;
    bool grounded = ownZ <= hipAbovegroundZMin;

    //reset velocity vertical when grounded, cap min 0
    //VERY IMPORTANT!
    if(grounded){
        float z = velocity.Z;
        velocity.Z = std::max(z, 0.0f);
    }


    //DEBUG
    //grounded = true;

    return grounded;
}

void HipController::Tick(float deltatime){
    //TickLocomotion(deltatime);

    FString message;
    if (groundedByDistance()) //new here with locomotion enabled!
    {

        if(locoMotionStateEnabled()){
            if(DEBUG_SLOWTIME){
                float timesSlower = 5.0f;
                deltatime = 1.0f / (timesSlower * 60.0f); // DEBUG
            }
                
    
            TickHipRotation(deltatime);
            TickLocomotion(deltatime);
            message = TEXT("locomotion");
        }else{
            RebuildLegsEndInPlace(deltatime);
        }
    }
    else
    {
        TickFalling(deltatime);
        message = TEXT("falling");
    }

    //DebugHelper::showScreenMessage(message);
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
    }
    else
    {
        attachment.TickBackwardKinematic(
            translation, 
            orientation,
            deltatime
        );
    }

    /*
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
    return;*/

    //old
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

    //DEBUG
    //targetLocal = orientation * targetLocal;

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


    //UNKLAR!
    //NEW HERE: REGISTER VELOCITY KILL 
    bool ignored = groundedByDistance();


    if(false){
        DebugHelper::logMessage("hipcontroller: backward start ", localStart);
        DebugHelper::logMessage("hipcontroller: backward end ", localEnd);
    }


    //PRE CALCULATE D 
    
    //A: end effector current relative to hip
    //B: end effector lift off, when next step touches ground relative to hip -- immer der default aber rückwärts?
    //komischer hack, muss genau definiert werden!

    FVector b = forwardDefaultLocalLocomotionFrame;
    float velocityDown = velocity.Z;
    attachment.setupSlipDataOnStanceBegin(
        orientation,
        b, // local end on liftoff - is flipped internally
        dynamicMotionTime,//motionTime,
        velocityDown, //current velocity downwards to overcome
        bodyMass
    );


    //SETUP ROTATION
    if(!rotationTimeOverriden){
        hipRotationInterpolator.overrideTime(dynamicMotionTime);
        rotationTimeOverriden = true;
    }
    
}

void HipController::setupForwardInterpolation(){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;


    //move trajectory to future with velocity
    //FVector localTrajectory = forwardDefaultLocalLocomotionFrame;

    //CAUTION: NEW: Rotated trajectory copy, reset rotation
    FVector localTrajectory = forwardTrajectory();


    //move local trajectory into rotation space: apply velocity offset, 
    //rotate back

    // t motionTime
    //gx = a + t (b-a)
    FVector localTrjectoryRotatedSpace = orientation * localTrajectory;
    localTrjectoryRotatedSpace += motionTime * velocity; //s * m/s = m 
    //achtung hier noch unklar: velocity auch drehen? nein, in rotated space!

    MMatrix orientationInverse = orientation.transposedRotation();
    localTrajectory = orientationInverse * localTrjectoryRotatedSpace;


    //move trajectory to world space, from hip starting joint
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
    
    //debug show trajectories.
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
    //FCollisionQueryParams collisionParams;
    //collisionParams.bTraceComplex = false;

    collisionParams.bTraceComplex = false;
    bool bHit = worldPointer->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, collisionParams);

    // If the raycast hit something, save the collision point
    if (bHit)
    {
        FVector worldImpact = HitResult.ImpactPoint;
        worldTarjectory = worldImpact;
        latestGroundTruth = worldImpact;
    }
}

void HipController::updateCollisionParams(FCollisionQueryParams Params){
    collisionParams = Params;
}

/// ---- force section ----
void HipController::applyForces(float deltatime){

    // es ist noch nicht klar ob die slip force an die gravity geknüpft wird,
    // eigentlich ja.
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
    //DebugHelper::showScreenMessage("velocity ", velocity, FColor::Orange);

    applyMaxVelocity();

    //x(t) = x0 + v0t + fällt weg(0.5at^2)?
    FVector x0 = translation.getTranslation();
    FVector xt = x0 + velocity * deltatime;
    validateTransformUpdate(xt);

    //DebugHelper::showScreenMessage("x(t) ", xt);
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
    FVector moveDir = lookDirection();

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
            SlipContainer &container = currentBoneAttachment->slipData(orientation);

            /**
             * was hier noch nicht klar sit ob die kraft auch 
             * in das welt koordintane system gebracht werden muss.
             * 
             * JA MUSS ES, IN ROTATION SPACE 
             */
            FVector accelerationCurrent = container.accelerationInterpolated(
                deltatime,
                bodyMass,
                moveDir
            );

            acceleration += accelerationCurrent;
        }
    }

    bool b = AccelerationIsSliding(acceleration);
    if(b){
        DebugHelper::logMessage("Slip: HipController: CAUTION Gliding: ", acceleration);
    }

    //x(t) = x0 + v0t + 0.5at^2
    //v(t) = v0 + at
    //a(t) = a
    //DebugHelper::showScreenMessage("acceleration ", acceleration, FColor::Orange);
    velocity += acceleration * deltatime;



    
}

bool HipController::AccelerationIsSliding(FVector &accelertation){
    MMatrix rInv = orientation.transposedRotation();
    FVector localAcceleration = rInv * accelertation;

    //dont allow gliding if rotation is not setup
    //is a symptom fix, target rotation matrix on legs seems to be bugged sometimes,
    //creating unexpected yaw rotation.
    bool bRemoveGlide = true && !rotationSet;
    if(bRemoveGlide){
        //could also be damped with 1/x
        FVector clamped = localAcceleration;

        //das stimmt so nicht, NUR Y, X ist local FORWARD!
        //clamped.X = 0.0f;
        clamped.Y = 0.0f;
        accelertation = orientation * clamped; //override, move world rotation space.
    }

    float epsilon = 0.00001f;
    return std::abs(localAcceleration.Y) > epsilon; //nur Y. Nicht X, local X ist forward.
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
    
    //vTarget = v
    //mGiven / x = vTarget
    //mGiven = vTarget * x
    //mGiven / vTarget = x

    float m = FVector::Dist(localStart, localEnd);
    float v = horizontalVelocity();

    float epsilon = 0.001f; //1cm s
    if (v < epsilon)
    {
        return motionTime; // default fallback
    }

    float time = m / v;
    DebugHelper::showScreenMessage("hipcontroller dynamic time backward ", (float)time);








    return time;
}





// ----- rotation section experimental ------
FVector HipController::forwardTrajectory(){
    FVector localTrajectory;
    bool newRotatedTrajectory = true; //debug, false block is deprecated, rotation trajectories are supported.
    if (newRotatedTrajectory)
    {
        localTrajectory = forwardRotatedLocalLocomotionFrame;

        //reset rotation of trajectory after step (interpolator setup now.)
        forwardRotatedLocalLocomotionFrame = forwardDefaultLocalLocomotionFrame;
    }
    else
    {
        localTrajectory = forwardDefaultLocalLocomotionFrame;
    }
    return localTrajectory;
}


FVector HipController::lookDirection(){
    FVector forwardDefault(1.0, 0.0, 0.0);
    FVector look = orientation * forwardDefault;
    return look.GetSafeNormal(); //for safety
}

void HipController::LookAt(FVector &location){
    //AB = B - A
    FVector lookDirTargeted = location - translation.getTranslation();
    lookDirTargeted = lookDirTargeted.GetSafeNormal();

    FVector lookDir = lookDirection();
    FVector2D look2D(lookDir.X, lookDir.Y);
    FVector2D targetLook2D(lookDirTargeted.X, lookDirTargeted.Y);
    if(true && FVector2D::DotProduct(look2D, targetLook2D) >= 0.90f){
        return;
    }

    float angle = MMatrix::signedAngleRadBetween(look2D, targetLook2D);
    setupRotationForNextStep(angle);
}

/// @brief rotate next step by a angle in rad
/// @param radian angle in
void HipController::setupRotationForNextStep(float radian){
    if(rotationSet){
        return;
    }
    DebugHelper::showScreenMessage("hipRotation start!", FColor::Red);

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

    //prevent skelleton from slipping and not running again properly
    slowDownBasedOnRotationInRadian(radian);


    // --- EXPERIMENTAL ---
    //tell legs here where to new local forward is targeted
    //later reset
    FVector dir(1, 0, 0);
    dir = addYawMat * dir;
    legLeft.updateLocalForwardMovingDirection(dir);
    legRight.updateLocalForwardMovingDirection(dir);

}


/// @brief will scale velocity immidiate based on radian to rotate in,
/// must be raw delta rotation add, velocity scalar is made internally
/// -- prevents skelleton from slipping into weird directions if velocity is to high
/// (Wird als stolpern wahrgenommen, unkontrolliert, fängt sich nicht)
/// @param radianInNextStep 
void HipController::slowDownBasedOnRotationInRadian(
    float radianInNextStep
){

    // --- prüfen ---
    // ----- rotate velocity and scale based on change in cos(angle) in (0,1) ------
    bool rotatevelocity = true;

    FVector forward = lookDirection();
    MMatrix deltaRotation;
    deltaRotation.yawRadAdd(radianInNextStep);
    FVector forwardDeltaApplied = deltaRotation * forward;
    float scalar = FVector::DotProduct(forward, forwardDeltaApplied);
    float slow = scalar > 0.0f ? scalar : 0.0f;

    //alternativ:
    //kann man radiant (pi/2) auf mit x / pi zu skalaren umwandeln?

    if(rotatevelocity){
        DebugHelper::logMessage("HipController update hVeloity on rotation before ", velocity);
        FVector updatedVelocity2D = forwardDeltaApplied.GetSafeNormal() * velocity.Size() * slow;
        updateHorizontalVelocity(updatedVelocity2D);
        DebugHelper::logMessage("HipController update hVeloity on rotation after ", velocity);
        //return;
    }

    //draw targeted rotation from hip for better understanding!
    bool bDebugShowRotationTargeted = true;
    if(bDebugShowRotationTargeted){
        //current
        DebugHelper::showLineBetween(
            worldPointer,
            forward * 100.0f + GetLocation(),
            GetLocation(),
            FColor::Orange,
            1.0f);

        //targeted
        DebugHelper::showLineBetween(
            worldPointer,
            forwardDeltaApplied * 100.0f + GetLocation(),
            GetLocation(),
            FColor::Purple,
            1.0f
        );
    }
    

    // ----- creates sliding glitch: may not be wanted ------
    /*    
    / *
    $$
    slow =\begin{cases}
        s_v \textit{ if } s_v> 0 \\
        0 \textit{ } otherwise 
    \end{cases} 
    $$
    * /
    float slow = scalar > 0 ? scalar : 0.0f;

    / *
    $$
    horizontal_{velocity} = horizontal_{velocity} \cdot slow
    $$
    * /
    //Horizontal XY Only!
    velocity.X *= slow;
    velocity.Y *= slow;

    DebugHelper::logMessageFloat("HipController: Slow down:", (float) slow);*/
}

void HipController::updateHorizontalVelocity(FVector &velocityIn){
    velocity.X = velocityIn.X;
    velocity.Y = velocityIn.Y;
}

/// @brief ticks the hip rotation if an angle was setup in "setupRotationForNextStep()"
/// @param deltatime 
void HipController::TickHipRotation(float deltatime){
    //debug
    //return;

    if(rotationSet && anyBackwardPhase() && rotationTimeOverriden){
        if(hipRotationInterpolator.endReached()){ //endReached(),
            rotationSet = false;
            rotationTimeOverriden = false;

            //DebugHelper::logMessage("hipRotation finished!");
            DebugHelper::showScreenMessage("hipRotation finished!");

            //reset leg orientation / forward local vector 
            //to tell legs ("we are aligned again, no roll yaw adjustment needed anymore")
            legLeft.resetLocalForwardMovingDirection();
            legRight.resetLocalForwardMovingDirection();


            //reset locomotionflag if requested
            if(locomotionStopRequestedOnceRotationIsFinished){
                locomotionStopRequestedOnceRotationIsFinished = false;
                stopLocomotion();
            }

            return;
        }

        FRotator rTicked = hipRotationInterpolator.interpolate(deltatime);
        MMatrix newRotation(rTicked);
        orientation = rTicked;

        DebugHelper::logMessage("hipRotation yaw: ", (float) rTicked.Yaw);
    }
}





// ---- motion api ----
void HipController::setStateWalking(){
    currentControllerState = EHipControllerStates::ELocomotion;
}

void HipController::stopLocomotion(){
    currentControllerState = EHipControllerStates::EIdle;
}

void HipController::stopLocomotionOnceRotationHasFinished(){
    locomotionStopRequestedOnceRotationIsFinished = true;
}

bool HipController::locoMotionStateEnabled(){
    return currentControllerState == EHipControllerStates::ELocomotion;
}



// --- api for get actors:apply damaged owner casted mesh actor ---
void HipController::getActors(TArray<AActor *> &outArray){
    legLeft.getActors(outArray);
    legRight.getActors(outArray);
}







// -------------- EXPERIMENTAL ------------------


// ---- external override ---- (just player)
void HipController::forceOverrideRotation(FRotator &rotation){
    orientation.setRotation(rotation);

    //speed may be removed, unclear if player will run on physics!
}