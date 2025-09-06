#include "AnimCarriedItemSocket.h"


AnimCarriedItemSocket::AnimCarriedItemSocket(){

}

AnimCarriedItemSocket::~AnimCarriedItemSocket(){

}

void AnimCarriedItemSocket::Tick(
    float deltatime,
    MMatrix &translation,
    MMatrix &orientation //orientation of actor or even combined with limb or camera look direction.
){
    if(itemIsAttached()){
        TickAnimation(deltatime);
        //Super tick after transform inner updated
        CarriedItemSocket::Tick(
            deltatime,
            translation,
            orientation
        );
    }
}

void AnimCarriedItemSocket::TickAnimation(float DeltaTime){
    
    //ticked in any case - if reached, stays in place, thats okay until changed
    if(
        localLocationInterpolator.TargetSetupFlag() &&
        !localLocationInterpolator.hasReachedTarget()
    ){
        translationCopy = localLocationInterpolator.interpolate(DeltaTime);
        //DebugHelper::showScreenMessage("Anim Socket Update FVectorInterpolator t: ", translationCopy);
        setLocalLocation(translationCopy);
    }


    if(
        localRotationInterpolator.TargetSetupFlag() &&
        !localRotationInterpolator.hasReachedTarget()
    ){
        rotationCopy = localRotationInterpolator.interpolate(DeltaTime);
            
        setLocalRotation(rotationCopy);
    }
    
    
}

///@brief move to a location relative to the inner offset
void AnimCarriedItemSocket::TryMoveToLocal(FVector &pos, float timeFrame){
    if(localLocationInterpolator.hasReachedTarget()){ //true even if no setup
        localLocationInterpolator.setTarget(
            translationCopy, // updated each frame, always up to date
            pos,
            timeFrame
        );
        //prints same position on aim (?)
        //DebugHelper::showScreenMessage("Anim Socket Update FVectorInterpolator FROM", translationCopy);
        //DebugHelper::showScreenMessage("Anim Socket Update FVectorInterpolator TO", pos);
    }
}

///@brief rotate relative to the inner socket rotation
void AnimCarriedItemSocket::TryRotateToLocal(FRotator &rotation, float timeFrame){
    if(localRotationInterpolator.hasReachedTarget()){ //true even if no setup
        localRotationInterpolator.setTarget(
            rotationCopy, // updated each frame, always up to date
            rotation,
            timeFrame
        );

        if(bLogMessage){
            DebugHelper::showScreenMessage("Anim Socket Update FRotatorInterpolator ");
        }
        
        
    }
}



///@brief move to a location relative to the inner offset
void AnimCarriedItemSocket::MoveToLocal(FVector &pos, float timeFrame){
    if(localLocationInterpolator.hasReachedTarget()){
        TryMoveToLocal(pos, timeFrame);
    }else{
        //-- not tested --
        localLocationInterpolator.overrideEndSpeedRelative(pos);
        //DebugHelper::showScreenMessage("Anim Socket FVectorInterpolator override End Speed relative");
        if(bLogMessage){
            DebugHelper::showScreenMessage("Anim Socket Update FRotatorInterpolator Fast");
        }
        
    }
}

///@brief rotate relative to the inner socket rotation
void AnimCarriedItemSocket::RotateToLocal(FRotator &rotation, float timeFrame){
    //localRotationInterpolator.overrideEndSpeedRelative(rotation);

    if(localRotationInterpolator.hasReachedTarget()){
        TryRotateToLocal(rotation, timeFrame);
    }else{
        //-- not tested --
        localRotationInterpolator.overrideEndSpeedRelative(rotation);
    }
}