#include "HipController.h"
#include "DebugPlugin/DebugHelper.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/SlipContainer.h"
#include "IkHumanoidModell/Ik/Controller/HipController/AnimationTime/AnimationTime.h"
#include "IkHumanoidModell/Ik/Controller/Properties/LimbProperties.h"

#include "IkHumanoidModell/Ik/Controller/ControllerSetup/FHumanoidControllerSetupPackage.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FTwoLimbHipProperty.h"

HipController::HipController(){
    rotationSet = false;
    worldPointer = nullptr;
}

HipController::~HipController(){
    worldPointer = nullptr;
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
    UpdatePlueckerJointRotation();
}

void HipController::setup(FHumanoidControllerSetupPackage &package){
    worldPointer = package.GetWorld();

    FTwoLimbHipProperty &legPropertyLeft = package.GetLegLeft();
    legLeft.setupBone(legPropertyLeft);

    FTwoLimbHipProperty &legPropertyRight = package.GetLegRight();
    legRight.setupBone(legPropertyRight);


    setupLegLength = legPropertyLeft.GetFirstAndSecondSize();

    //setup forward reach target
    float lengthTotal = std::abs(setupLegLength);
    forwardDefaultLocalLocomotionFrame = FVector(40.0f, 0, -lengthTotal);
    DebugHelper::logMessage("Default Forward Trajectory unprojected", forwardDefaultLocalLocomotionFrame);
    forwardRotatedLocalLocomotionFrame = forwardDefaultLocalLocomotionFrame;

    locomotionProperty = package.GetLocomotionProperty();
    buildOnStart();
    SetupPlueckerJoint(package.GetWorld());
}

void HipController::buildOnStart(){

    //build bones once
    float deltatime = 1.0f;
    MMatrix root = translation * orientation; //<-- lese richtung --
    legLeft.TickNone(root, deltatime);
    legRight.TickNone(root, deltatime);

    setupForwardInterpolation();
}



bool HipController::HipAtGroundLevel(){
    float hipAbovegroundZMin = latestGroundTruth.Z;
    float ownZ = translation.getTranslation().Z;
    bool grounded = ownZ <= hipAbovegroundZMin;
    return grounded;
}

bool HipController::groundedByDistance(){
    /*if(anyBackwardPhase()){
        return true;
    }*/

    //reset velocity vertical when grounded, cap min 0
    //VERY IMPORTANT!
    if(HipAtGroundLevel()){
        float z = velocity.Z;
        velocity.Z = std::max(z, 0.0f);
        return true;
    }
    return false;
}



void HipController::Tick(float deltatime){
    if(collapseEnabledFlag){
        TickCollapsePhysics(deltatime);
        return;
    }

    //TickLocomotion(deltatime);
    showExtendedDebugLog(deltatime);
    
    if(bDebugBlockTick){
        return;
    }
    UpdateStanceStatus();

    

    if(locoMotionStateEnabled()){
        if(DEBUG_SLOWTIME){
            float timesSlower = 5.0f;
            deltatime = 1.0f / (timesSlower * 60.0f); // DEBUG
        }
            

        TickHipRotation(deltatime);
        TickLocomotion(deltatime);
        //message = TEXT("locomotion");
    }else{

        RebuildLegsEndInPlace(deltatime);
    }

    /*
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
        
        //makes it worse.
        //ResetAllLocomotionFlags(); //doesnt fix infinite fall issue
    }*/

    //DebugHelper::showScreenMessage(message);

    /*
    DebugHelper::showScreenMessage(
        FString::Printf(
            TEXT("z height %.2f, v_z %.2f"), 
            GetLocation().Z, 
            velocity.Z
        )
    );*/
}

void HipController::ResetAllLocomotionFlags(){
    legLeftPlaying = true;
    forwardMotion = true;
}

void HipController::TickLocomotion(float deltatime){
    updateInterpolatorLocomotion(deltatime);
    applyLocomotion(deltatime);
    applyForces(deltatime);
}

void HipController::TickFalling(float deltatime){

    applyForceGravity(deltatime);
    applyVelocity(deltatime);
    RebuildLegsEndInPlace(deltatime);
    //RebuildLegsNone(deltatime);

    
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

    
    BoneAttachment &other = !legLeftPlaying ? legLeft : legRight;
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


bool HipController::backwardsKinematicAllowed(){
    if(!forwardMotion){
        BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;
        return attachment.EndEffectorIsGrounded();
    }
    return false;
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
        if(!backwardsKinematicAllowed()){
            //DebugHelper::showScreenMessage("backwards kinematic not allowed!", FColor::Red);
            //return;
        }

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
    float dynamicMotionTime = AnimationTime::AnimationTimeBasedOnHorizontalVelocity(
        localStart, localEnd, horizontalVelocity()
    );
    
    
    
    interpolatorBackwardLocal.setTarget(localStart, localEnd, dynamicMotionTime);

    //SETUP ROTATION
    if(!rotationTimeOverriden){
        hipRotationInterpolator.overrideTime(dynamicMotionTime);
        rotationTimeOverriden = true;
    }
    
    setupBackwardInterpolation(dynamicMotionTime);
}

//base method
void HipController::setupBackwardInterpolation(float dynamicMotionTime){
    //keep empty. not needed here.
    DebugHelper::logMessage("HipController Base setupBackwardInterpolation");
}

void HipController::setupForwardInterpolation(){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;


    //move trajectory to future with velocity
    FVector localTrajectory = forwardTrajectory();
    ApplyVelocityToLocalTrajectory(localTrajectory);

    //move trajectory to world space, from hip starting joint
    FVector worldTrajectory = attachment.inWorldSpace(
        localTrajectory, //forwardDefaultLocalLocomotionFrame,
        translation,
        orientation
    );

    //project frame to ground
    FVector worldTrajectoryProjected = worldTrajectory;
    projectToGround(worldTrajectoryProjected);
    attachment.UpdateGroundTruth(worldTrajectoryProjected);

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

    // ----- new ------
    float dynamicMotionTime = AnimationTime::AnimationTimeBasedOnHorizontalAndVerticalVelocity(
        currentEndEffector, 
        worldTrajectoryProjected, 
        horizontalVelocity(),
        verticalVelocity()
    );


    //no gravity at all. More consitent.
    dynamicMotionTime = motionTime;

    //copy for IK hand interface
    FlagUpdatedMotionTimeForArmAnimation(dynamicMotionTime);

    interpolatorForwardWorld.setHermiteSplineFlag(true);
    interpolatorForwardWorld.setTarget(
        currentEndEffector,
        worldTrajectoryProjected,
        dynamicMotionTime
        // motionTime
    );

    //interpolatorForwardWorld.setTarget(currentEndEffector, worldTrajectoryProjected, motionTime);
}


void HipController::ApplyVelocityToLocalTrajectory(FVector &localTrajectory){
    //move local trajectory into rotation space: apply velocity offset, 
    //rotate back, returns local trajectory with velocity added to future

    // t motionTime
    //gx = a + t (b-a)
    FVector localTrjectoryRotatedSpace = orientation * localTrajectory;
    localTrjectoryRotatedSpace += motionTime * velocity; //s * m/s = m 
    //achtung hier noch unklar: velocity auch drehen? nein, in rotated space!

    MMatrix orientationInverse = orientation.transposedRotation();
    localTrajectory = orientationInverse * localTrjectoryRotatedSpace;
}

void HipController::RemoveVelocityFromWorldTrajectory(FVector &worldTrajectory){
    //Sepcial function just for second trajectory for slip backward setup
    //with regareds to second leg

    //M = T * R <-- lese richtung
    //M^-1 = R^-1 * T^-1
    MMatrix translationInverse = translation.invertedTranslation();
    MMatrix orientationInverse = orientation.transposedRotation();
    MMatrix inv = orientationInverse * translationInverse;
    FVector localTrajectory = inv * worldTrajectory;

    localTrajectory -= motionTime * velocity; //s * m/s = m

    MMatrix M = translation * orientation;
    worldTrajectory = M * localTrajectory;
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
    bool bHit = worldPointer->LineTraceSingleByChannel(
        HitResult, 
        Start, 
        End, 
        ECC_Visibility, 
        //collisionParams,
        GetCollisionParams()
    );

    // If the raycast hit something, save the collision point
    if (bHit)
    {
        FVector worldImpact = HitResult.ImpactPoint;
        worldTarjectory = worldImpact;
        latestGroundTruth = worldImpact;
    }
}

FCollisionQueryParams HipController::GetCollisionParams(){
    FCollisionQueryParams params = ASharedRaycastParamManager::getCollisonParams();
    params.bTraceComplex = false;
    return params;
}

/// ---- force section ----
void HipController::applyForces(float deltatime){

    //unklar ob es hier bleibt
    applyForceGravity(deltatime);
    applySlipForce(deltatime);
    applyVelocity(deltatime);
    
    //very important
    //rebuild both to update end effectors - is needed in case of backward kinematic.
    RebuildLegsEndInPlace(deltatime);
}

void HipController::applySlipForce(float deltatime){
    applySlipForceStatic(deltatime);
}


void HipController::applyVelocity(float deltatime){

    //debug
    //DebugHelper::showScreenMessage("velocity ", velocity, FColor::Orange);

    ClampMaxVelocity();

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



void HipController::RebuildLegsNone(float deltatime){
    MMatrix transform = translation * orientation;
    legLeft.TickNone(
        transform,
        deltatime
    );
    legRight.TickNone(
        transform,
        deltatime
    );
}






//hack
void HipController::ClampMaxVelocity(){
    FVector2D velocity2D(velocity.X, velocity.Y);
    //float maxHorizontalVelocity = 200.0f;

    float maxHorizontalVelocity = locomotionProperty.GetMaxVelocity();

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




void HipController::applySlipForceStatic(float deltatime){
    FVector move = lookDirection();

    FVector v1 = legLeft.StaticSlipVelocity(
        move,
        verticalVelocity(),
        bodyMass,
        deltatime,
        leftInStancePhase()
    );

    FVector v2 = legRight.StaticSlipVelocity(
        move,
        verticalVelocity(),
        bodyMass,
        deltatime,
        rightInStancePhase()
    );

    float div = 1.0f;
    if (v1.Size() > 0.0f && v2.Size() > 0.0f)
    {
        div = 2.0f;
    }

    FVector vSum = v1 + v2;
    velocity += vSum / div;
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


void HipController::RemoveSlidingFromVector(FVector &accelertation){
    MMatrix rInv = orientation.transposedRotation();
    FVector localAcceleration = rInv * accelertation;

    float epsilon = 0.00001f;
    bool bIsSliding = std::abs(localAcceleration.Y) > epsilon;

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

    if(bIsSliding){
        DebugHelper::logMessage("Slip: HipController: CAUTION Gliding: Local:", localAcceleration);
    }
}

void HipController::applyForceGravity(float deltatime){
    float gravityScale = 1.0f;
    FVector acceleration(0.0f, 0.0f, -981.0f * gravityScale);
    velocity += acceleration * deltatime;
}




float HipController::horizontalVelocity(){
    FVector copy = velocity;
    copy.Z = 0.0f;
    return copy.Size();
}

float HipController::verticalVelocity(){
    return velocity.Z;
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
    // FRotator TransformInterpolator::interpolateRotationOnly(float DeltaTime)

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
        //DebugHelper::logMessage("HipController update hVeloity on rotation before ", velocity);
        FVector updatedVelocity2D = forwardDeltaApplied.GetSafeNormal() * velocity.Size() * slow;
        updateHorizontalVelocity(updatedVelocity2D);
        //DebugHelper::logMessage("HipController update hVeloity on rotation after ", velocity);
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

            //DebugHelper::showScreenMessage("hipRotation finished!");

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
        UpdatePlueckerJointRotation();

        //DebugHelper::logMessage("hipRotation yaw: ", (float) rTicked.Yaw);
    }
}





// ---- motion api ----
void HipController::setStateWalking(){
    currentControllerState = EHipControllerStates::ELocomotion;
}

void HipController::stopLocomotion(){
    currentControllerState = EHipControllerStates::EIdle;

    //debug: remove velocity

}

void HipController::stopLocomotionOnceRotationHasFinished(){
    locomotionStopRequestedOnceRotationIsFinished = true;
}

bool HipController::locoMotionStateEnabled(){
    return currentControllerState == EHipControllerStates::ELocomotion &&
           !bDebugBlockLocomotion;
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
    UpdatePlueckerJointRotation();
    //speed may be removed, unclear if player will run on physics!
}


void HipController::ResetAndRebuild(){
    SetStateCollapse(false);
    FRotator none;
    orientation.setRotation(none);
    UpdatePlueckerJointRotation();

    legLeft.ResetAndRebuild(translation, orientation);
    legRight.ResetAndRebuild(translation, orientation);
    //from header
    forwardMotion = true;
    legLeftPlaying = true;
    forwardRotatedLocalLocomotionFrame = forwardDefaultLocalLocomotionFrame;
    rotationSet = false;


    FString message = TEXT("HumanoidController Rebuild: HipController: Rebuild Hip ");
    message += GetLocation().ToString();
    DebugHelper::logMessage(message);

    DebugHelper::showLineBetween(
        worldPointer,
        GetLocation(),
        GetLocation() + FVector(100, 0, 0),
        FColor::Yellow,
        100.0f
    );
}

// -------------- DEBUG FOR SYMPTOMS ----------------

void HipController::EnableDebugLogExtended(){
    bExtendedDebugLog = true;
    
    //bDebugBlockLocomotion = true;
    //bDebugBlockTick = true;
}

void HipController::showExtendedDebugLog(float deltatime){
    if(bDebugBlockTick){
        return;
    }

    if(bExtendedDebugLog){
        debugIntegratedTime += deltatime;
        if(debugIntegratedTime < debugIntervall){
            return;
        }
        debugIntegratedTime = 0.0f;

        if(groundedByDistance()){
            FString msg = TEXT("HipController extended DebugLog - Is Grounded");
            DebugHelper::showScreenMessage(msg);
        }else{
            FString msg = TEXT("HipController extended DebugLog - Is NOT Grounded");
            DebugHelper::showScreenMessage(msg);
        }

        DebugIsInGround();

        if(!locoMotionStateEnabled()){
            if(bExtendedDebugLog){
                FString msg = TEXT("HipController extended DebugLog - ");
                msg += TEXT("No locomotion: Horizonal Velocity");
                msg += FString::Printf(TEXT("%.2f"),horizontalVelocity());
                DebugHelper::logMessage(msg);
            }
        }
        if(locoMotionStateEnabled()){
            if(bExtendedDebugLog){
                float velocityHorizonal = horizontalVelocity();
                FString msg = TEXT("HipController extended DebugLog - ");
                msg += TEXT("locomotion: Horizonal Velocity");
                msg += FString::Printf(TEXT("%.2f"), velocityHorizonal);
                DebugHelper::logMessage(msg);

                bool bBlockVelocity = true;
                float maxVelocity = 0.1f;
                if (velocityHorizonal > maxVelocity)
                {
                    FVector vCopy = velocity;
                    bool dampNeeded = DebugHorizontalVelocityOvershoot();
                    FVector vCopy1 = velocity;
                    
                    FString messageV = TEXT("HipController extended DebugLog - ");
                    messageV += TEXT("Removed Y from Velocity");
                    messageV += vCopy.ToString();
                    messageV += TEXT(" -> ");
                    messageV += vCopy1.ToString();

                    float yaw = orientation.extractRotator().Yaw;
                    messageV += FString::Printf(TEXT(" yaw (%.2f)"), yaw);

                    DebugHelper::showScreenMessage(messageV);

                    if(dampNeeded){
                        FVector copy = velocity;
                        float scalar = std::min(maxVelocity, velocityHorizonal);
                        velocity = velocity.GetSafeNormal() * scalar;
                        velocity.Z = copy.Z;

                        FString messageDamped = TEXT("HipController extended DebugLog - ");
                        messageDamped += TEXT("Damped Velocity");
                        messageDamped += velocity.ToString();
                        DebugHelper::showScreenMessage(messageDamped);
                    }
                }
            }
            //is fast here, 20.0 cm / s
        }
    }
}

void HipController::DebugIsInGround(){
    return;

    if(IsInGround()){
        FVector current = GetLocation();
        SetLocation(current);

        FString msg = TEXT("HipController extended DebugLog - Fix Ground Stuck");
        DebugHelper::showScreenMessage(msg);
    }
}

bool HipController::IsInGround(){
    float currentZ = GetLocation().Z;
    return std::abs(currentZ - latestGroundTruth.Z) <= 100;
}

bool HipController::DebugHorizontalVelocityOvershoot(){
    //move horizontal velocity to local rotation space
    FVector v = velocity;
    FVector copy = v;
    v.Z = 0.0f;
    MMatrix Rinv = orientation.transposedRotation(); //scheinbar richtige rotation
    FVector vLocal = Rinv * v;

    //remove right left velocity
    vLocal.Y = 0.0f;

    //move back to world rotation
    velocity = orientation * vLocal;

    //override
    velocity.Z = copy.Z;

    if(FVector::DistSquared(velocity, copy) <= 100){ //10cm^2
        return true;
    }

    return false;
}







/// ---- pluecker joints ----
void HipController::SetupPlueckerJoint(UWorld *world){

    rootJoint = RootJoint(GetLocation(), world);




    //concatenate child
    rootJoint.AddChildByPointer(legRight.GetTopJoint());
    rootJoint.AddChildByPointer(legLeft.GetTopJoint());
    
    //set constraints on hip joints:
    SetPlueckerHipConstraint(legRight);
    SetPlueckerHipConstraint(legLeft);


    //rootJoint.MarkAs6DOF(true); //for gravity.
    //FJointConstraint &constraintOfJoint = rootJoint.GetConstraint();
    //constraintOfJoint.UnLockPositionConstraint(); //unlock constraint
}

void HipController::SetPlueckerHipConstraint(BoneAttachment &attachment){
    if(Joint *boneAttachmentJoint = attachment.GetTopJoint()){
        FJointConstraint &constraint = boneAttachmentJoint->GetConstraint();
        constraint.allowRollRotation = false; //100% false
        constraint.allowPitchRotation = false;
        // pitch unclear
    }
}



Joint *HipController::GetTopJoint(){
    return &rootJoint;
}

void HipController::UpdatePlueckerJointRotation(){
    rootJoint.OverrideJointRotation(orientation);
}





void HipController::SetRotation(const MMatrix &other){
    orientation.setRotation(other);
}


void HipController::ReactToDamage(const FCustomHitResult &hitResult){
    legLeft.ReactToDamage(hitResult);
    legRight.ReactToDamage(hitResult);
}

//--- enable /disable collapse physics ---
void HipController::SetStateCollapse(bool flag){
    if(!isAlreadyCollapsed() && flag){
        UpdateRootJointOnCollapse();
    }
    IJointInterface::SetStateCollapse(flag);
    legLeft.SetStateCollapse(flag);
    legRight.SetStateCollapse(flag);
    
    BeginAngularDampingTimerCollapsPhysics();
}

void HipController::UpdateRootJointOnCollapse(){
    rootJoint.OverrideWorldLocation(translation.getTranslation());
    rootJoint.OverrideLinearVelocity(velocity);
}





void HipController::TickCollapsePhysics(float deltatime){
    
    FCollisionQueryParams params = ASharedRaycastParamManager::getCollisonParams();
    rootJoint.UpdateIgnoreParamsRecursive(params);
    rootJoint.TickAndBuildRecursive(deltatime);
    TickAngularDampingTimerCollapsePhysics(deltatime);
    TickAngularDampingTimerCollapsePhysics(deltatime);

    //rootJoint.logGroundedState("HipController::", FColor::Cyan);
}

void HipController::BeginAngularDampingTimerCollapsPhysics(){
    angularDampingTimer.Begin(angularDampingTimeToFullDamp, false);
}

void HipController::TickAngularDampingTimerCollapsePhysics(float deltatime){
    if(angularDampingTimer.timesUp()){
        return;
    }

    angularDampingTimer.Tick(deltatime);
    float scalar = angularDampingTimer.InvertedScalar(); //between 1 and 0, damp towards zero
    DebugHelper::logMessage(FString::Printf(TEXT("HipController::AngularDamp %.2f"), scalar));
    rootJoint.SetAngularDampingRecursive(scalar);
}

FVector HipController::GetVelocity(){
    return velocity;
}

FVector2D HipController::GetHorizontalVelocity(){
    return FVector2D(velocity.X, velocity.Y);
}




void HipController::FlagUpdatedMotionTimeForArmAnimation(float dynamicMotionTimeInForward){
    //forward and backward is same right now
    float dynamicMotionTimeInForwardAndBackward = dynamicMotionTimeInForward * 2.0f;

    updatedMotionTime = dynamicMotionTimeInForwardAndBackward;
    bMotionTimeUpdatedForArms = true;
}

bool HipController::HasMotionTimeUpdate(float &outMotionTime){
    bool copy = bMotionTimeUpdatedForArms;
    bMotionTimeUpdatedForArms = false;
    if (copy){
        outMotionTime = updatedMotionTime;
    }
    return copy;
}




void HipController::AppendFootPositions(TArray<FVector> &outPositions){
    outPositions.Add(legLeft.endEffectorWorldLocation());
    outPositions.Add(legRight.endEffectorWorldLocation());
}