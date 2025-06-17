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
}

void HipController::setup(UWorld *world){
    worldPointer = world;

    float lengthA = 50.0f;
    float lengthB = 50.0f;

    //x is forward
    FVector offsetLeft(0, -20.0f, 0.0f);
    legLeft.setupBone(lengthA, lengthB, world, offsetLeft);

    FVector offsetRight = offsetLeft * -1.0f;
    legRight.setupBone(lengthA, lengthB, world, offsetRight);



    //setup forward reach target
    float lengthTotal = std::abs(lengthA) + std::abs(lengthB);
    float distanceForward = lengthTotal * 0.2f;
    forwardDefaultLocalLocomotionFrame = FVector(distanceForward, 0, -lengthTotal);

    //debug
    forwardDefaultLocalLocomotionFrame = FVector(distanceForward, 0, -lengthTotal * 0.7f);

    forwardDefaultLocalLocomotionFrame = FVector(distanceForward * 2.0f, 0, -lengthTotal * 0.7f);

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

void HipController::Tick(float deltatime){
    TickLocomotion(deltatime);

    //ground flag noch instabil.
    /*
    if(isGrounded()) 
        TickLocomotion(deltatime);
    else
        TickFalling(deltatime);
    */
    // drawLocation(deltatime);
}

void HipController::TickLocomotion(float deltatime){
    updateInterpolatorLocomotion(deltatime);
    applyLocomotion(deltatime);
    applyForces(deltatime);
}

void HipController::TickFalling(float deltatime){
    applyForceGravity(deltatime);
    applyVelocity(deltatime);
    MMatrix transform = translation * orientation; //<-- lese richtung --
    legLeft.TickNone(transform, deltatime);
    legRight.TickNone(transform, deltatime);
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

    DebugHelper::logMessage("hipcontroller: backwards start ", localStart);
    DebugHelper::logMessage("hipcontroller: backwards end ", localEnd);    

    interpolatorBackwardLocal.setTarget(localStart, localEnd, motionTime);



    //wenn das bein den boden berührt und bis zum heel
    //off am boden bleibt gilt für das bein bis zum nächsten forward 
    //die stance phase
    stancePhaseLegLeft = legLeftPlaying;


}



void HipController::setupForwardInterpolation(){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;


    //move trajectory to future with velocity
    FVector localTrajectory = forwardDefaultLocalLocomotionFrame;

    //TESTING NEEDED

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

    DebugHelper::logMessage("hipcontroller: forward start ", currentEndEffector);
    DebugHelper::logMessage("hipcontroller: forward end ", worldTrajectoryProjected);
    DebugHelper::showLineBetween(
        worldPointer, currentEndEffector, currentEndEffector + FVector(0, 0, 30), FColor::Orange, 1.0f
    );
    DebugHelper::showLineBetween(
        worldPointer, currentEndEffector, worldTrajectoryProjected , FColor::Yellow, 1.0f
    );

    interpolatorForwardWorld.setTarget(currentEndEffector, worldTrajectoryProjected, motionTime);
}


void HipController::projectToGround(FVector &worldTarjectory){
    if(!worldPointer){
        return;
    }

    FVector Start = worldTarjectory + FVector(0, 0, 200);
    FVector End = worldTarjectory + FVector(0, 0, -3000);


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
    applyForceGravity(deltatime);
    
    if(isGrounded()){
        velocity.Z = std::max(velocity.Z, 0.0);
        DebugHelper::showScreenMessage("velocity reset ", FColor::Red);
    }
    //applyForceGravity(deltatime);
    applyVelocity(deltatime);
}

//testing needed
bool HipController::isGrounded(){
    if(forwardMotion && legLeftPlaying){
        return legRight.reachedTarget();
    }
    if(forwardMotion && !legLeftPlaying){
        return legLeft.reachedTarget();
    }
    return false;
}

void HipController::applyVelocity(float deltatime){

    //debug
    DebugHelper::showScreenMessage("velocity ", velocity);
    

    //x(t) = x0 + v0t + fällt weg(0.5at^2)?
    FVector x0 = translation.getTranslation();
    FVector xt = x0 + velocity * deltatime;
    validateTransformUpdate(xt);

    DebugHelper::showScreenMessage("x(t) ", xt);
    translation.setTranslation(xt);
}

/// @brief clamps the position update to not fall below ground
/// @param position 
void HipController::validateTransformUpdate(FVector &position){
    float heightUpdate = std::max(position.Z, latestGroundTruth.Z);
    position.Z = heightUpdate;
}

void HipController::applyStancePhaseSLIPForce(float deltatime){
    //stance phase ist eigentlich fast immer true
    //wenn man normal geht
    //immer ein bein eben!
    
    BoneAttachment &attachment = stancePhaseLegLeft ? legLeft : legRight;

    //if the foot is actually grounded apply force
    SlipContainer container = attachment.slipData();
    
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

    FVector moveDir(1, 0, 0);
    moveDir = orientation * moveDir;


    //x(t) = x0 + v0t + 0.5at^2
    //v(t) = v0 + at
    //a(t) = a

    FVector acceleration = container.acceleration(bodyMass, moveDir);
    velocity += acceleration * deltatime;
}

void HipController::applyForceGravity(float deltatime){
    //apply gravity if not below
    //ground 
    //maybe by adding extra distance (but not sure) to keep from ground

    float gravityScale = 1.0f; //0.4

    float currentZheight = translation.getTranslation().Z;
    float groundZheight = latestGroundTruth.Z;

    if(currentZheight > groundZheight){
        FVector acceleration(0, 0, -981 * gravityScale);
        velocity += acceleration * deltatime;
    }
}