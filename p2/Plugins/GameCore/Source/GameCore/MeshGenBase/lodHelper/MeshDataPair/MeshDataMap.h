#pragma once

#include "CoreMinimal.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"
#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include <map>

#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshDataMap/TMeshDataMap.h"

class ProceduralMeshComponentPair; // forward declaration

/// @brief base class to store meshdata for raycast enabled and non enabled layers
class GAMECORE_API MeshDataMap : public TMeshDataMap<materialEnum> {
    //Allow ProceduralMeshComponentPair derived from this, using the protected members of another object.
    friend class ProceduralMeshComponentPair;

public:
    MeshDataMap();
    ~MeshDataMap();

};