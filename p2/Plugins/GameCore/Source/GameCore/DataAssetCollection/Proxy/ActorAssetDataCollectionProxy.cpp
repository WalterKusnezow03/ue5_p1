#include "ActorAssetDataCollectionProxy.h"
#include "GameCore/DataAssetCollection/ActorAssetDataCollection.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "AssetEnumCollection/assetEnums/EDataAssetEnum.h"
#include "GameCore/DataAssetCollection/ActorAssetByString.h"

UActorAssetDataCollection *ActorAssetDataCollectionProxy::FindInstance(){
    if(assetManager *a = assetManager::instance()){
        if(UActorAssetDataCollection* pack = a->Find<EDataAssetEnum, UActorAssetDataCollection>(
            EDataAssetEnum::ActorDataAssetPack
        )){
            return pack;
        }
    }
    return nullptr;
}


void ActorAssetDataCollectionProxy::SpawnAllByName(
    UWorld *world,
    FString name,
    const TArray<FVector> &positions
){
    if(UActorAssetDataCollection *collection = FindInstance()){
        collection->SpawnAllByName(world, name, positions);
    }
}

void ActorAssetDataCollectionProxy::SpawnAllByName(
    UWorld *world,
    FString name,
    const TArray<FVector> &positions,
    TArray<AActor*> &outSpawned
){
    if(UActorAssetDataCollection *collection = FindInstance()){
        collection->SpawnAllByName(world, name, positions, outSpawned);
    }
}

FActorAssetByString *ActorAssetDataCollectionProxy::FindAsset(FString name){
    if(UActorAssetDataCollection *collection = FindInstance()){
        return collection->FindAsset(name);
    }
    return nullptr;
}