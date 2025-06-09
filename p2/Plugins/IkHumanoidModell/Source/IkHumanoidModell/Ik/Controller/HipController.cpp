#include "HipController.h"
#include "GameCore/DebugHelper.h"

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
    drawLocation(deltatime);
}

void HipController::TickLocomotion(float deltatime){
    updateInterpolatorLocomotion(deltatime);
    applyLocomotion(deltatime);

}

//apply locomotion
void HipController::applyLocomotion(float deltatime){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;
    MMatrix transform = translation * orientation; //<-- lese richtung --
    if(forwardMotion){
        attachment.TickForwardKinematic(transform, deltatime);

        FString message = legLeftPlaying ? TEXT("leg 1 forward") : TEXT("leg 2 forward ");
        DebugHelper::showScreenMessage(message);
    }
    else
    {
        attachment.TickBackwardKinematic(
            translation, 
            orientation,
            deltatime
        );

        FString message = legLeftPlaying ? TEXT("leg 1 backward") : TEXT("leg 2 backward ");
        DebugHelper::showScreenMessage(message);
    }


    BoneAttachment &other = !legLeftPlaying ? legLeft : legRight;
    other.TickNone(transform, deltatime);
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
}



void HipController::setupForwardInterpolation(){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;

    //move trajectory to world space
    FVector worldTrajectory = attachment.inWorldSpace(
        forwardDefaultLocalLocomotionFrame,
        translation,
        orientation
    );

    //project frame to ground
    FVector worldTrajectoryProjected = worldTrajectory;

    //set as target with end and current Starting point (end effector)
    FVector currentEndEffector = attachment.endEffectorWorldLocation();

    DebugHelper::logMessage("hipcontroller: forward start ", currentEndEffector);
    DebugHelper::logMessage("hipcontroller: forward end ", worldTrajectoryProjected);

    DebugHelper::showLineBetween(
        worldPointer, currentEndEffector, currentEndEffector + FVector(0, 0, 30), FColor::Red, 1.0f
    );

    interpolatorForwardWorld.setTarget(currentEndEffector, worldTrajectoryProjected, motionTime);
}


