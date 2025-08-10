#include "HumanoidController.h"

HumanoidController::HumanoidController(){
    FVector location(100, 0, 100);
    mainItemSocket.setLocalLocation(location);

}

HumanoidController::~HumanoidController(){

}

FVector HumanoidController::GetLocation(){
    return hipController.GetLocation();
}

FVector HumanoidController::lookDirection(){
    return hipController.lookDirection();
}

void HumanoidController::SetLocation(FVector &target){
    hipController.SetLocation(target);
}

void HumanoidController::defaultSetup(UWorld *world){
    float legPartSizeEach = 50;
    float armPartSizeEach = 40;
    float torsoHeight = 50;
    float torsoHalfWidth = 30;

    hipController.setup(world);
    torsoController.setup(
        torsoHeight,
        torsoHalfWidth,
        armPartSizeEach,
        armPartSizeEach,
        world
    );

}

void HumanoidController::Tick(float deltatime){

    hipController.Tick(deltatime);

    torsoController.Tick(
        hipController.getTranslation(), // MMatrix &actorTranslation,
        hipController.getOrientation(), // MMatrix &actorRotation,
        deltatime
    );
    TickMainCarriedItemSocket(deltatime);
}

void HumanoidController::TickMainCarriedItemSocket(float deltatime){
    MMatrix rotation = hipController.getOrientation(); //maybe modified if needed

    mainItemSocket.Tick(
        deltatime,
        hipController.getTranslation(),
        rotation //orientation of actor or even combined with limb or camera look direction.
    );
}



// --- Attach api ---

void HumanoidController::attachOrReplaceCarriedItem(IIkCarryInterface *newItem){
    //remove old item
    if(IIkCarryInterface *previousItem = mainItemSocket.attachedItemPointer()){
        updateCollisionParams(previousItem, false); //remove old item
    }

    torsoController.attachOrReplaceCarriedItem(newItem);
    mainItemSocket.attachOrReplaceCarriedItem(newItem);
    updateCollisionParams(newItem, true); //add new item
}

void HumanoidController::raycastIgnoreOwner(AActor *actor){
    updateCollisionParams(actor, true);
}

void HumanoidController::updateCollisionParams(IIkCarryInterface *ptr, bool add){
    if(ptr != nullptr){
        // exclude this actor from the trace if possible, otherwise none
        AActor *casted = Cast<AActor>(ptr);
        if(casted){
            updateCollisionParams(casted, add);
        }
    }
}

void HumanoidController::updateCollisionParams(AActor *actor, bool add){
    if(actor != nullptr){
        if(add){
            collisionParamsProvider.AddIgnoredActor(actor);
        }else{
            collisionParamsProvider.RemoveIgnoredActor(actor);
        }
        //update params since anything has changed
        hipController.updateCollisionParams(collisionParamsProvider.getCollisonParams());
    }
}




void HumanoidController::dropCarriedItem(){
    //remove from collision params
    updateCollisionParams(mainItemSocket.attachedItemPointer(), false); //remove

    //drop
    torsoController.dropCarriedItem();
    mainItemSocket.dropCarriedItem();
}

//rotation change
void HumanoidController::LookAt(FVector &location){
    hipController.LookAt(location);
}

void HumanoidController::setupRotationForNextStep(float radianYaw){
    hipController.setupRotationForNextStep(radianYaw);
}

void HumanoidController::forceYawAdd(float degree){
    hipController.forceYawAdd(degree);
}


void HumanoidController::setStateWalking(){
    hipController.setStateWalking();
}

void HumanoidController::stopLocomotion(){
    hipController.stopLocomotion();
}





//--- socket change api ---
void HumanoidController::changeCarriedItemSocket(ECarriedItemPosition type){
    //Todo: Add Carried item socket interpolator / map
}