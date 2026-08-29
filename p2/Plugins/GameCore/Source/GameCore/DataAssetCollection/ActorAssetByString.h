#pragma once

#include "CoreMinimal.h"

#include "ActorAssetByString.generated.h"

/// @brief struct to save any UCLASS Asset (primarily for AActor classes to spawn)
/// - will allow to store actors / Bp's per String name
/// and spawn them accordingly
USTRUCT(BlueprintType)
struct GAMECORE_API FActorAssetByString
{
    GENERATED_BODY()

public:
    FActorAssetByString() {};

    UPROPERTY(EditAnywhere, Category="ActorAssetData")
    FString nameOfAsset;

    UPROPERTY(EditAnywhere, Category="ActorAssetData")
    float spawnOnePerCmQuad; //per cm on one axis of the quad of cm^2

    //Actor to be spawned
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ActorAssetData")
    TSubclassOf<AActor> SpawnClass;

    bool NameEqualsIgnoreCase(FString &other) const;

    AActor *SpawnActor(UWorld *world) const;
    AActor *SpawnActor(UWorld *world, FVector SpawnLocation) const;

    //the spawned actors will be appended to array
    void SpawnActors(UWorld *world, const TArray<FVector> &locations, TArray<AActor*> &outActors) const;
};