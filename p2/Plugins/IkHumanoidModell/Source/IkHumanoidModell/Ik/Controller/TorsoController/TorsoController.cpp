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

    FTwoLimbProperty &spinePackage = package.GetTorsoSpineSize();
    spine.setup(spinePackage);
    spineTarget = FVector(0, 0, spinePackage.GetFirstAndSecondSize()); //up ward default target

    partLeft.setup(
        EArmType::ELeft,
        package
    );

    partRight.setup(
        EArmType::ERight,
        package
    );
    headController.setup(package);
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
    if(collapseEnabledFlag){ //physics managed by hip.
        return;
    }

    //build spine
    BuildSpine(actorTranslation, actorRotation, deltatime);
    MMatrix spineWorldTranslation = spine.EndEffectorTranslation();

    //build top torso - not tested!
    partLeft.Tick(spineWorldTranslation, actorRotation, deltatime);
    partRight.Tick(spineWorldTranslation, actorRotation, deltatime);
    headController.Tick(spineWorldTranslation, actorRotation, deltatime);
}

void TorsoController::BuildSpine(
    const MMatrix &actorTranslation,
    const MMatrix &actorRotation,
    float deltatime
){
    MMatrix worldTransform = actorTranslation * actorRotation; // M = T * R <-- lese richtung --
    spine.MoveToTarget(spineTarget, worldTransform, deltatime);
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
    spine.getActors(outArray);
    headController.getActors(outArray);
}

// ---- pluecker joints ----
void TorsoController::SetupJointParents(){
    //connect shoulders to spine end (upper end)
    spine.AddChildsToLowerJoint(GetTopJointsOfLayeredArms());

    spine.AddChildToLowerJoint(headController.GetTopJoint());
}

//internal use of start effector joints of layered two joint bone to connect spine
//to shoulders.
TArray<Joint *> TorsoController::GetTopJointsOfLayeredArms(){
    TArray<Joint *> outArray;
    outArray.Add(partLeft.GetTopJoint());
    outArray.Add(partRight.GetTopJoint());
    return outArray;
}



//lower spine starting joint to connect to hip as child.
Joint *TorsoController::GetTopJoint(){
    return spine.GetTopJoint(); //lower spine location (returns p1.)
}






void TorsoController::ReactToDamage(const FCustomHitResult &hitResult){
    spine.ReactToDamage(hitResult);
    partLeft.ReactToDamage(hitResult);
    partRight.ReactToDamage(hitResult);
    headController.ReactToDamage(hitResult);
}


void TorsoController::SetStateCollapse(bool flag){
    IJointInterface::SetStateCollapse(flag);
    spine.SetStateCollapse(flag);
    partLeft.SetStateCollapse(flag);
    partRight.SetStateCollapse(flag);
    headController.SetStateCollapse(flag);
}
