#include "TorsoController.h"
#include "EArmType.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/IKCarryInterfaceAnimatedActor.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/FHumanoidControllerSetupPackage.h"

TorsoController::TorsoController(){

}

TorsoController::~TorsoController(){

}


void TorsoController::setup(FHumanoidControllerSetupPackage &package){
    partLeft.setup(
        EArmType::ELeft,
        package
    );

    partRight.setup(
        EArmType::ERight,
        package
    );
    SetupJointParents();
}

void TorsoController::ResetAndRebuild(
    MMatrix &actorTranslation,
    MMatrix &actorRotation
){
    //M = T * R <-- lese richtung --
    //MMatrix M = actorTranslation * actorRotation;
    Tick(actorTranslation, actorRotation, 0.0f); //lazy rebuild, must be refactured.

    FString message = TEXT("HumanoidController Rebuild: TorsoController: Rebuild Torso ");
    message += actorTranslation.getTranslation().ToString();
    DebugHelper::logMessage(message);
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

//api for get actors:apply damaged owner casted mesh actor
void TorsoController::getActors(TArray<AActor *> &outArray){
    partLeft.getActors(outArray);
    partRight.getActors(outArray);
}






// ---- pluecker joints ----
void TorsoController::SetupJointParents(){
    partLeft.SetParentInterface(this);
    partRight.SetParentInterface(this);
}


void TorsoController::UpstreamPropagate(
    FJointKinematicPropagatePackage &package
){
    //TODO
}

void TorsoController::DownstreamPropagate(
    FJointKinematicPropagatePackage &package
){
    DownstreamPropagateTo(partLeft, package);
    DownstreamPropagateTo(partRight, package);
}

void TorsoController::DownstreamPropagateTo(
    LayeredTwoJointBone &attachment,
    FJointKinematicPropagatePackage &package
){
    attachment.DownstreamPropagate(package);
}