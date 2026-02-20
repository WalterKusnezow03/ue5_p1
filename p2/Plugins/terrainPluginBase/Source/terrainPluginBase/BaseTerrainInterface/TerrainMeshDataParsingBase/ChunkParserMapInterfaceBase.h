#pragma once
#include "CoreMinimal.h"

#include "terrainPluginBase/BaseTerrainInterface/TerrainMeshDataParsingBase/ChunkParserInterfaceBase.h"


/// @brief INTERFACE ONLY
class TERRAINPLUGINBASE_API ChunkParserMapInterfaceBase {

public:

    //TO BE OVERRIDEN
    virtual ChunkParserInterfaceBase &findByIndexBase(int i, int j) = 0;

private:

};