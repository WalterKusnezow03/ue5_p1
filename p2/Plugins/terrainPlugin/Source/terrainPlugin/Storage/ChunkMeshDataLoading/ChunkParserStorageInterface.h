#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/TerrainMeshDataParser/ChunkParser.h"
#include "StoragePlugin/Storage/VertexData/StorageInterface/StorageInterfaceMeshData.h"
#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"


/// @brief will load and save ChunkParser files for custom mesh actor, by passing
/// references which are filled or read from
class TERRAINPLUGIN_API ChunkParserStorageInterface : public StorageInterfaceMeshData {

public:
    ChunkParserStorageInterface();
    ~ChunkParserStorageInterface();

    //save single chunk. Will load all mesh data and materials, lods automatically
    void Save(FString worldLevelname, ChunkParser &chunkData);
    bool Load(FString worldLevelname, ChunkParser &chunkData);

private:
    int materialToId(materialEnum type);
};