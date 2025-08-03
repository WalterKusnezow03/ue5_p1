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
    partLeft.Tick(actorTranslation, actorRotation, hipPitch, deltatime);
    partRight.Tick(actorTranslation, actorRotation, hipPitch, deltatime);
}

MMatrix TorsoController::applyUpRotation(MMatrix &inRotation){
    MMatrix result = inRotation * hipPitch; //<-- lese richtung --
    return result;
}

void TorsoController::SetWorldTargetLeft(FVector &target){

}

void TorsoController::SetWorldTargetRight(FVector &target){

}


void TorsoController::attachOrReplaceCarriedItem(IIkCarryInterface *ptr){
    partLeft.attachOrReplaceCarriedItem(ptr);
    partRight.attachOrReplaceCarriedItem(ptr);
}