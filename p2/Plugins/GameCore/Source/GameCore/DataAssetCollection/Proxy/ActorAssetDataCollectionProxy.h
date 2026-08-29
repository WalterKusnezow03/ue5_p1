#pragma once

#include "CoreMinimal.h"

class UActorAssetDataCollection;
struct FActorAssetByString;

class GAMECORE_API ActorAssetDataCollectionProxy {

public:

    static void SpawnAllByName(
        UWorld *world,
        FString name,
        const TArray<FVector> &positions
    );

    static void SpawnAllByName(
        UWorld *world,
        FString name,
        const TArray<FVector> &positions,
        TArray<AActor*> &outSpawned
    );

    static FActorAssetByString *FindAsset(FString name);

private:

    static UActorAssetDataCollection *FindInstance();


};
