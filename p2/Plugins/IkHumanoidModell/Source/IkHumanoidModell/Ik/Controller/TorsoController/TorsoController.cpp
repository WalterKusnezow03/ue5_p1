#include "TorsoController.h"
#include "EArmType.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"

TorsoController::TorsoController(){

}

TorsoController::~TorsoController(){

}

void TorsoController::setup(
    float hipBreast,
    float breastShoulder,
    float upperArm,
    float lowerArm,
    UWorld *world
){
    partLeft.setup(
        EArmType::ELeft,
        hipBreast,
        breastShoulder,
        upperArm,
        lowerArm,
        world
    );

    partRight.setup(
        EArmType::ERight,
        hipBreast,
        breastShoulder,
        upperArm,
        lowerArm,
        world
    );

    hipPitch.pitchRadAdd(MMatrix::degToRadian(180));
}

void TorsoController::Tick(
    MMatrix &actorTranslation,
    MMatrix &actorRotation, 
    float deltatime
){
    partLeft.Tick(actorTranslation, actorRotation, deltatime);
    partRight.Tick(actorTranslation, actorRotation, deltatime);
}


void TorsoController::SetWorldTargetLeft(FVector &target){

}

void TorsoController::SetWorldTargetRight(FVector &target){

}


void TorsoController::attachOrReplaceCarriedItem(IIkCarryInterface *ptr){
    partLeft.attachOrReplaceCarriedItem(ptr);
    partRight.attachOrReplaceCarriedItem(ptr);
}

void TorsoController::dropCarriedItem(){
    partLeft.dropCarriedItem();
    partRight.dropCarriedItem();
}