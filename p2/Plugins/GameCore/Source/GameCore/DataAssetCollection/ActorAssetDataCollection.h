#pragma once

#include "CoreMinimal.h"

#include "ActorAssetDataCollection.generated.h"

/// @brief saves a set of FActorAssetByString
UCLASS(BlueprintType)
class GAMECORE_API UActorAssetDataCollection : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="AssetData")
    TArray<FActorAssetByString> assetData;

    void SpawnAllByName(
        UWorld *world,
        FString name,
        const TArray<FVector> &positions
    ) const;

    void SpawnAllByName(
        UWorld *world,
        FString name,
        const TArray<FVector> &positions,
        TArray<AActor*> &outSpawned
    ) const;

    

    FActorAssetByString *FindAsset(FString name);
};
