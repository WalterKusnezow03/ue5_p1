#pragma once
#include "CoreMinimal.h"

#include "terrainPluginBase/BaseTerrainInterface/TerrainMeshDataParsingBase/ChunkParserInterfaceBase.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"

/// @brief INTERFACE ONLY
class TERRAINPLUGINBASE_API ChunkParserMapInterfaceBase {

public:

    //TO BE OVERRIDDEN
    virtual ChunkParserInterfaceBase &findByIndexBase(int i, int j) = 0;

    ChunkParserInterfaceBase &findByWorldLocation(TerrainInterfaceBase *creator, const FVector &pivot){
        if(creator){
            std::pair<int, int> posIndex2D = creator->Index2DFromWorldPosition(pivot);
            ChunkParserInterfaceBase &interfaceFound = findByIndexBase(posIndex2D.first, posIndex2D.second);
            return interfaceFound;
        }
        return findByIndexBase(0, 0);
    }

private:

};