#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/ELod.h"

/// @brief INTERFACE ONLY
class TERRAINPLUGINBASE_API ChunkParserInterfaceBase {

public:

    //TO BE OVERRIDEN
    virtual MeshData &findMeshDataReference(
        materialEnum type,
        ELod lodLevel,
        bool raycastOnLayer
    ) = 0;

private:

};