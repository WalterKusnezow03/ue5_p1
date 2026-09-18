#include "NNResultFlagManager.h"
#include "p2/entityManager/OutPost/Flag/FlagActor.h"


NNResultFlagManager::NNResultFlagManager(){
    
}

NNResultFlagManager::~NNResultFlagManager(){
    
}

NNResultFlagManager::NNResultFlagManager(const NNResultFlagManager &other){
    if(this != &other){
        *this = other;
    }
}

NNResultFlagManager &NNResultFlagManager::operator=(const NNResultFlagManager &other){
    if(this != &other){
        ClearFlags();
        flags = other.flags;
    }
    return *this;
}

void NNResultFlagManager::ClearFlags(){
    for (int i = 0; i < flags.Num(); i++){
        if(AFlagActor *current = flags[i]){
            current->despawn();
        }
    }
    flags.Empty();
}

void NNResultFlagManager::UpdateFlagPositions(const TArray<FVector> &positions, UWorld *world){
    TArray<FVector> copy = positions;
    FVector dirScaled(0,0,-10000);
    raycaster.projectAll(world, copy, dirScaled, false);
    UpdateFlagPositions(copy);
}

void NNResultFlagManager::UpdateFlagPositions(const TArray<FVector> &positions){
    ClearFlags();
    for (int i = 0; i < positions.Num(); i++){
        if(AFlagActor *instanceMade = AFlagActor::MakeInstanceAt(positions[i])){
            flags.Add(instanceMade);
            //DebugHelper::logMessage("NNResultFlagManager::Flag Made");
        }else{
            //DebugHelper::logMessage("NNResultFlagManager::NoFlag Made");
        }
    }
}


void NNResultFlagManager::DrawFlagPositionsFrom(FVector actorLocation, UWorld *world){
    //debug draw
    if(!world){
        return;
    }
    for (int i = 0; i < flags.Num(); i++){
        if(AActor *current = flags[i]){
            
            DebugHelper::showLineBetween(
                world,
                current->GetActorLocation(),
                actorLocation,
                FColor::Green,
                1.0f
            );
        }

        
    }
}