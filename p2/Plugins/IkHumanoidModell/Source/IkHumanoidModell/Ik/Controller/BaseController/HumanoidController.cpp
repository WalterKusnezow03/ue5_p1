#include "HumanoidController.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "IkHumanoidModell/SharedRaycastParams/SharedRaycastParamManager.h"
#include "IkHumanoidModell/Ik/Controller/Properties/LimbProperties.h"

HumanoidController::HumanoidController(){
    FVector location(100, 0, 100);
    mainItemSocket.setLocalLocation(location);

}

HumanoidController::~HumanoidController(){

}

void HumanoidController::EnableDebugLogExtended(){
    hipController.EnableDebugLogExtended();
}

FVector HumanoidController::GetLocation(){
    return hipController.GetLocation();
}

FRotator HumanoidController::GetRotation(){
    return hipController.getOrientation().extractRotator();
}

FVector HumanoidController::lookDirection(){
    return hipController.lookDirection();
}

void HumanoidController::SetLocation(FVector &target){
    hipController.SetLocation(target);
    //rebuild torso once
    torsoController.Tick(
        hipController.getTranslation(), // MMatrix &actorTranslation,
        hipController.getOrientation(), // MMatrix &actorRotation,
        0.0f
    );
}

void HumanoidController::ResetAndRebuild(){
    TickMainCarriedItemSocket(0.0f);
    hipController.ResetAndRebuild();
    torsoController.ResetAndRebuild(
        hipController.getTranslation(), // MMatrix &actorTranslation,
        hipController.getOrientation() // MMatrix &actorRotation,
    );
}

void HumanoidController::defaultSetup(UWorld *world){
    
    
    float torsoHeight = 50;
    float torsoHalfWidth = 30;

    LimbProperties::GetTorsoProperties(torsoHalfWidth, torsoHeight);

    float armPartSizeEach = 40;
    LimbProperties::GetSizeArmLimb(armPartSizeEach);

    hipController.setup(world);
    torsoController.setup(
        torsoHeight,
        torsoHalfWidth,
        armPartSizeEach,
        armPartSizeEach,
        world
    );
    SetupEmptyArmAnimationActor(world);
    addAllActorsInChildrenToRaycastExclude();
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
        if(previousItem == newItem){
            return;
        }

        updateCollisionParams(previousItem, false); //remove old item
    }

    //update flag if is empty actor item
    if(emptyArmTargetActor){
        emptyActorIsPickedUp = emptyArmTargetActor == newItem;
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
            ASharedRaycastParamManager::Add(actor);
        }else{
            ASharedRaycastParamManager::Remove(actor);
        }
        hipController.updateCollisionParams(ASharedRaycastParamManager::getCollisonParams());



        //deprecated
        /*
        if(add){
            collisionParamsProvider.AddIgnoredActor(actor);
        }else{
            collisionParamsProvider.RemoveIgnoredActor(actor);
        }
        //update params since anything has changed
        hipController.updateCollisionParams(collisionParamsProvider.getCollisonParams());
        */
    }
}




void HumanoidController::dropCarriedItem(){
    if(emptyActorIsPickedUp){
        return;
    }


    //remove from collision params
    updateCollisionParams(mainItemSocket.attachedItemPointer(), false); //remove

    //drop
    torsoController.dropCarriedItem();
    mainItemSocket.dropCarriedItem();

    OnDropUpdateAnimation();
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

void HumanoidController::stopLocomotionOnceRotationHasFinished(){
    hipController.stopLocomotionOnceRotationHasFinished();
}

//--- apply damaged owner in bone mesh actors ---
//set damaged owner api
void HumanoidController::setDamagedOwner(IDamageinterface *damagedOwnerIn){
    if(damagedOwnerIn){
        //find all components in children, try cast actor to mesh actor base

        int counted = 0;
        TArray<AActor *> children = actorInChildrenArray();
        for (int i = 0; i < children.Num(); i++){
            AActor *current = children[i];
            if (current){

                //try cast to mesh actor base which has a damaged owner
                AcustomMeshActorBase *casted = Cast<AcustomMeshActorBase>(current);
                if(casted){
                    casted->setDamagedOwner(damagedOwnerIn);
                    counted++;
                }
            }
        }
        //DebugHelper::logMessage("HumanoidController: set damage in children", counted);
    }
}


void HumanoidController::addAllActorsInChildrenToRaycastExclude(){
    TArray<AActor *> children = actorInChildrenArray();
    for (int i = 0; i < children.Num(); i++){
        if(AActor *current = children[i]){
            bool add = true; //track
            updateCollisionParams(current, add);
        }
    }
}

TArray<AActor *> HumanoidController::actorInChildrenArray(){
    TArray<AActor *> actorArray;
    hipController.getActors(actorArray);
    torsoController.getActors(actorArray);
    return actorArray;
}




//--- socket change api ---
void HumanoidController::changeCarriedItemSocket(ECarriedItemPosition type){
    //Todo: Add Carried item socket interpolator / map
}



void HumanoidController::defaultSetupHands(UWorld *worldIn){
    torsoController.defaultSetupHands(worldIn);
}










//--- drop item / empty animation item ---

void HumanoidController::SetupEmptyArmAnimationActor(UWorld *world){
    if(world && !emptyArmTargetActor){
        emptyArmTargetActor = AIKCarryInterfaceAnimatedActor::makeInstance(world);
        OnDropUpdateAnimation();
    }
}


void HumanoidController::OnDropUpdateAnimation(){
    if(emptyArmTargetActor){
        DebugHelper::logMessage("HumanoidController::Pickup Empty Actor");
        
        attachOrReplaceCarriedItem(emptyArmTargetActor);
        emptyArmTargetActor->FireAnimation(EArmAnimationEnum::running);
        
    }
}
