#include "worldLevelBase.h"

#include "DebugPlugin/DebugHelper.h"

AworldLevelBase *AworldLevelBase::Instance = nullptr;

AworldLevelBase::AworldLevelBase(){
    PrimaryActorTick.bCanEverTick = true; 
}

void AworldLevelBase::MakeInstanceBase(UWorld *world){
    if(!Instance && world){
        AworldLevelBase *made = Make<AworldLevelBase>(world);
    }
}

void AworldLevelBase::BeginPlay(){
    Instance = this;
}

void AworldLevelBase::Tick(float deltatime){
    Super::Tick(deltatime);
}

void AworldLevelBase::EndPlay(const EEndPlayReason::Type EndPlayReason){
    Instance = nullptr;
    PlayerPointer = nullptr;
    Super::EndPlay(EndPlayReason);
}

void AworldLevelBase::SetPlayerReference(AActor *player){
    DebugHelper::logMessage("AworldLevelBase::SetPlayerReference");
    if (Instance)
    {
        if(player){
            if(Instance->PlayerPointer != player){
                Instance->PlayerPointer = player;
                Instance->OnPlayerReferenceSet();
            }
        }
    }
}

AActor *AworldLevelBase::GetPlayerReference(){
    if(Instance){
        return Instance->PlayerPointer;
    }
    return nullptr;
}

void AworldLevelBase::OnPlayerReferenceSet(){

}


void AworldLevelBase::addOutpostAt(FVector &pos){
    if(Instance){
        Instance->outpostsToCreate.Add(pos);
    }
}

