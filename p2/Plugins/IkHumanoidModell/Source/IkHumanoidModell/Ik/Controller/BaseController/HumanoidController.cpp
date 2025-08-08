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

void HumanoidController::attachOrReplaceCarriedItem(IIkCarryInterface *ptr){
    torsoController.attachOrReplaceCarriedItem(ptr);
    mainItemSocket.attachOrReplaceCarriedItem(ptr);
}

void HumanoidController::dropCarriedItem(){
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

