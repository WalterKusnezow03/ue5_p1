#include "HumanoidController.h"

HumanoidController::HumanoidController(){

}

HumanoidController::~HumanoidController(){

}

void HumanoidController::defaultSetup(UWorld *world){
    float legPartSizeEach = 50;
    float armPartSizeEach = 40;
    float torsoHeight = 40;
    float torsoHalfWidth = 20;

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
}

void HumanoidController::attachOrReplaceCarriedItem(IIkCarryInterface *ptr){
    torsoController.attachOrReplaceCarriedItem(ptr);
}
