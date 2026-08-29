#include "ActorAssetDataCollection.h"



void UActorAssetDataCollection::SpawnAllByName(
    UWorld *world,
    FString name,
    const TArray<FVector> &positions
) const {
    TArray<AActor *> ignored;
    SpawnAllByName(world, name, positions, ignored);
}

void UActorAssetDataCollection::SpawnAllByName(
    UWorld *world,
    FString name,
    const TArray<FVector> &positions,
    TArray<AActor*> &outSpawned
) const {
    for (int i = 0; i < assetData.Num(); i++){
        const FActorAssetByString &current = assetData[i];
        if(current.NameEqualsIgnoreCase(name)){
            current.SpawnActors(world, positions, outSpawned);
            return;
        }
    }
}

FActorAssetByString *UActorAssetDataCollection::FindAsset(FString name){
    for (int i = 0; i < assetData.Num(); i++){
        FActorAssetByString &current = assetData[i];
        if(current.NameEqualsIgnoreCase(name)){
            return &current;
        }
    }
    return nullptr;
}