#pragma once

#include "CoreMinimal.h"
#include "terrainPluginBase/BaseTerrainInterface/externalActorTask/ExternalActorSpawnCollection.h"
#include "StoragePlugin/Storage/BaseInterface/StorageInterface.h"


/// @brief will load and save ChunkParser files for custom mesh actor, by passing
/// references which are filled or read from
class TERRAINPLUGINBASE_API ExternalActorSpawnCollectionStorageInterface : public StorageInterface {

public:

    //save single chunk. Will load all mesh data and materials, lods automatically
    void Save(FString worldLevelname, ExternalActorSpawnCollection &collection);
    bool Load(FString worldLevelname, ExternalActorSpawnCollection &collection);

private:
    FString makePath(FString worldLevelName);

};