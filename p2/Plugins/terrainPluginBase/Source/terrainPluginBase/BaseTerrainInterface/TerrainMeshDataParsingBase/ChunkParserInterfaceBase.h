#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/ELod.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"

/// @brief INTERFACE ONLY
class TERRAINPLUGINBASE_API ChunkParserInterfaceBase {

public:

    //TO BE OVERRIDEN
    virtual MeshData &findMeshDataReference(
        materialEnum type,
        ELod lodLevel,
        bool raycastOnLayer
    ) = 0;

    //TO BE OVERRIDEN
    //targeted location
    virtual FVector GetActorLocation() = 0;

private:

};