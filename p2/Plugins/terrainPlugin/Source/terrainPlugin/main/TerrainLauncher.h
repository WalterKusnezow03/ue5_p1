#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/main/management/ActorManager.h"
#include "terrainPluginBase/BaseTerrainInterface/externalActorTask/ExternalActorSpawnCollection.h"
#include "TerrainLauncher.generated.h"

/// @brief will launch the terrain generation or try to load from save storage
UCLASS()
class TERRAINPLUGIN_API ATerrainLauncher : public AActor{

    GENERATED_BODY()

public:
    ATerrainLauncher();

    
    static ATerrainLauncher* makeInstance(
        UWorld *world, 
        FString WorldLevelName,
        const ExternalActorSpawnCollection &preparedSpawnCollection //can be empty.
    );

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float deltatime) override;

    //external play and end - does not kill this instance, aactors can be reused for loading another level!
    void BeginAndLoad(FString WorldLevelName);

    //the ExternalActorSpawnCollection will override the current one
    //and filled with the loaded collection from disk
    void BeginAndLoad(FString WorldLevelName, const ExternalActorSpawnCollection &spawnActorParams);
    void EndAndSave();

    //use to spawn entities / actors after terrain creation, or world loading
    const ExternalActorSpawnCollection &GetExternalActorCollection();

private:

    bool IsADifferentWorldThanCurrent(FString WorldLevelName);
    

    ActorManager actorManager;
    FString copiedWorldName = TEXT("None");
    bool wasInitedOnce = false;
};