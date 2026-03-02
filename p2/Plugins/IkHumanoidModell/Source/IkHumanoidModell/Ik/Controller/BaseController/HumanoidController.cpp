#include "HumanoidController.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "IkHumanoidModell/SharedRaycastParams/SharedRaycastParamManager.h"

#include "IkHumanoidModell/Ik/Controller/ControllerSetup/FHumanoidControllerSetupPackage.h"

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

void HumanoidController::defaultSetup(AActor *actorOwner){

    /*FHumanoidControllerSetupPackage property = FHumanoidControllerSetupPackage::GetDefault(world);

    hipController.setup(property);
    torsoController.setup(property);
    SetupEmptyArmAnimationActor(world);
    addAllActorsInChildrenToRaycastExclude();*/
    HumanoidController::defaultSetup(actorOwner, false);
}

void HumanoidController::defaultSetup(AActor *actorOwner, bool flagWantedHands){
    FHumanoidControllerSetupPackage property = FHumanoidControllerSetupPackage::GetDefault(actorOwner);
    if(flagWantedHands){
        property.MarkHandsWanted();
    }

    hipController.setup(property);
    torsoController.setup(property);
    SetupEmptyArmAnimationActor(property.GetWorld());
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
    //drop previous item from hand (needed here? unclear)
    dropItemFromEmptyActorHand();

    //remove old item if not same
    if(IIkCarryInterface *previousItem = CurrentPickedUpItem()){ //REFACTURE FOR METHOD
        if(newItem){
            if(previousItem == newItem){
                return;
            }
        }
        
        updateCollisionParams(previousItem, false); //remove old item
    }

    //update flag if is empty actor item
    /*if(emptyArmTargetActor){
        emptyActorIsPickedUp = emptyArmTargetActor == newItem;
    }*/

    //NEW for injected items
    //inject into empty Arm Target Actor if possible
    //instead of attaching as socket
    if(TryInjectIntoEmptyIkCarryInterface(newItem)){
        return;
    }

    //default
    torsoController.attachOrReplaceCarriedItem(newItem);
    mainItemSocket.attachOrReplaceCarriedItem(newItem);
    updateCollisionParams(newItem, true); //add new item

    //set flag
    UpdateEmptyArmTargetActorPickedUpFlag();
}

//not tested but shoudl be fine
IIkCarryInterface *HumanoidController::CurrentPickedUpItem(){
    if(IIkCarryInterface *item = mainItemSocket.attachedItemPointer()){
        return item;
    }
    return nullptr;
}

//not tested but should be fine 
bool HumanoidController::EmptyActorIsPickedUp(){
    if(emptyArmTargetActor){
        if(IIkCarryInterface *current = CurrentPickedUpItem()){
            return emptyArmTargetActor == current;
        }
    }
    return false;
}


//set flag
void HumanoidController::UpdateEmptyArmTargetActorPickedUpFlag(){

    if(emptyArmTargetActor){
        bool flagUpdate = EmptyActorIsPickedUp();
        emptyArmTargetActor->SetIsPickedUpFlag(flagUpdate);
    }
}



//PARTIALLY TESTED, BUGGED

/// @brief will try to inject the item to the empty hand animation actor
/// returns true on success. Item picked up via empty hands.
/// @param newItem 
/// @return 
bool HumanoidController::TryInjectIntoEmptyIkCarryInterface(IIkCarryInterface *newItem){
    if(emptyArmTargetActor){
        if(emptyArmTargetActor == newItem){
            return false;
        }
    }


    if(emptyArmTargetActor && newItem){
        if(newItem->GetCarryType() == EIKCarryType::ECarryByHand){
            //drop previous item
            dropItemFromEmptyActorHand();

            //testing needed
            DebugHelper::logMessage("HumanoidController::Injected Carry By Hand Item!");

            emptyArmTargetActor->InjectCarryByHandItem(newItem);
            updateCollisionParams(newItem, true); //add new item
            
            
            //attach empty actor since was injected
            torsoController.attachOrReplaceCarriedItem(emptyArmTargetActor);
            mainItemSocket.attachOrReplaceCarriedItem(emptyArmTargetActor);
            updateCollisionParams(Cast<AActor>(emptyArmTargetActor), true); //add new item
            UpdateEmptyArmTargetActorPickedUpFlag();
            // emptyActorIsPickedUp = true;

            return true;
        }
    }
    return false;
}

/// ---> throwing is unclear here because a item can be thrown
/// and also can be reloadable (?) Although somehow the item
/// must eject itself / notify the empty arm actor that it would
/// like to be thrown
/// the best way would be to make the item throwable itself
/// and reload one if possible (?)
/// architecture quiete unclear here.








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

//collision params for hip / leg down raycast, ground raycast
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
    //-- eject hand carried item --
    if(EmptyActorIsPickedUp() && emptyArmTargetActor){
        dropItemFromEmptyActorHand();

        //IIkCarryInterface *ejected = emptyArmTargetActor->CurrentAttachedItem();
        //updateCollisionParams(ejected, false); //remove from collision ignore
        //emptyArmTargetActor->EjectCarryByHandItem();
        return;
    }


    //-- eject socket carried item --

    //remove from collision params
    updateCollisionParams(mainItemSocket.attachedItemPointer(), false); //remove

    //drop
    torsoController.dropCarriedItem();
    mainItemSocket.dropCarriedItem();

    OnDropUpdateAnimation();
}

void HumanoidController::dropItemFromEmptyActorHand(){
    DebugHelper::logMessage("HumanoidController::dropItemFromEmptyActorHand A");
    if (emptyArmTargetActor)
    {
        DebugHelper::logMessage("HumanoidController::dropItemFromEmptyActorHand B");
        IIkCarryInterface *ejected = emptyArmTargetActor->CurrentAttachedItem();
        
        emptyArmTargetActor->EjectCarryByHandItem();
        if(ejected){
            updateCollisionParams(ejected, false); //remove from collision ignore
            DebugHelper::logMessage("HumanoidController::dropItemFromEmptyActorHand C, ejected real item");
        }
    }
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


//--- enable /disable collapse physics ---
void HumanoidController::SetStateCollapse(bool flag){
    collapseEnabledPhysics = flag;
    hipController.SetStateCollapse(flag);
    torsoController.SetStateCollapse(flag);

    if(flag && emptyArmTargetActor){
        emptyArmTargetActor->StopAnimation();
    }
}

void HumanoidController::SetStateCollapseTrue(){
    SetStateCollapse(true);
}







//--- drop item / empty animation item ---

void HumanoidController::SetupEmptyArmAnimationActor(UWorld *world){
    if(world && emptyArmTargetActor == nullptr){
        emptyArmTargetActor = AIKCarryInterfaceAnimatedActor::makeInstance(world);
        OnDropUpdateAnimation();
    }
}


void HumanoidController::OnDropUpdateAnimation(){
    if(emptyArmTargetActor){
        DebugHelper::logMessage("HumanoidController::Pickup Empty Actor");
        
        attachOrReplaceCarriedItem(emptyArmTargetActor);
        UpdateEmptyArmTargetActorPickedUpFlag();
        emptyArmTargetActor->FireAnimation(EArmAnimationEnum::running);
    }
}
