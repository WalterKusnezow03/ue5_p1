#pragma once

#include "CoreMinimal.h"


#include "GameCore/util/ActorBase/ActorBase.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"

#include "MeshDataPlugin/Public/Extraction/StaticMeshCopy/MeshExtractorBase.h"

/// @brief extracts meshdata from staticmesh render data
class MESHDATAPLUGIN_API MeshExtractor : public MeshExtractorBase{
public:

    void ExtractMeshDataPerSection(AActorBase *actor, TArray<MeshData> &data);
    void ExtractSingleMeshData(AActorBase *actor, MeshData &meshdata);
};