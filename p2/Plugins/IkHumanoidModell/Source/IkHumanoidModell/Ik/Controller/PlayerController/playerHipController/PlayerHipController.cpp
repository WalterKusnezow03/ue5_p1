#include "PlayerHipController.h"



void PlayerHipController::Tick(float deltatime){
    UpdateVelocity(deltatime);
    applyLocomotion(deltatime);
    if(HasMoved()){
        updateInterpolatorLocomotion(deltatime);
    }
}

void PlayerHipController::UpdateVelocity(float deltatime){
    //is needed for external velocity to have the player legs in correct place!

    //x(t) = x0 + v0t + (ignored 1/2 at^2)
    //x1 = x0 + v0t 
    //x1 - x0 = v0t
    //(x1 - x0)/t = v0
    FVector currentlocation = GetLocation();
    FVector moved = currentlocation - prevTickLocation;
    FVector v = moved / deltatime;
    v.Z = 0.0f;
    velocity = v;
}

bool PlayerHipController::HasMoved(){
    FVector currentlocation = GetLocation();
    float minDistance = 10.0f;
    if(FVector::DistSquared(currentlocation, prevTickLocation) >= minDistance){
        prevTickLocation = currentlocation;
        return true;
    }
    return false;
}

//changed for player
void PlayerHipController::updateInterpolatorLocomotion(float deltatime){
    //switch legs once forward interpolation is finished
    FVector targetWorld = interpolatorForwardWorld.interpolate(deltatime);
    ApplyMaxHeightForForwardTarjectory(targetWorld);
    updateForwardTargetWorld(targetWorld);
    if(interpolatorForwardWorld.hasReachedTarget()){
        legLeftPlaying = !legLeftPlaying;
        setupForwardInterpolation();
    }
}

void PlayerHipController::ApplyMaxHeightForForwardTarjectory(FVector &trajectory){
    double zHipCurrent = GetLocation().Z;
    trajectory.Z = std::min(trajectory.Z, zHipCurrent);
}

void PlayerHipController::applyLocomotion(float deltatime){
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;
    
    //new
    MMatrix t = getTranslationModfied(); // translation

    MMatrix transform = t * orientation; //<-- lese richtung --
    attachment.TickForwardKinematic(transform, deltatime);
    
    BoneAttachment &other = !legLeftPlaying ? legLeft : legRight;
    other.TickKeepEndInWorldPlace(
        t,
        orientation,
        deltatime
    );
}




//TRANSLATION BACKWARD OFFSET, shoulders are very in way
MMatrix &PlayerHipController::getTranslationModfied(){
    FVector backwardOffset(-10, 0, 0);
    MMatrix mLocalbackward(backwardOffset);
    MMatrix worldRotationSpace = getOrientation() * mLocalbackward; //M = R * T <-- lese richtung --

    MMatrix clearTranslation;
    clearTranslation.setTranslation(worldRotationSpace);

    MMatrix M = translation * clearTranslation;

    modifiedTranslation.setTranslation(M);
    return modifiedTranslation;
}
