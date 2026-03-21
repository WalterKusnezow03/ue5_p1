#pragma once

#include "CoreMinimal.h"

#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"

/// @brief extracts meshdata from staticmesh render data
class MESHDATAPLUGIN_API MeshExtractorBase {
public:

    

    void ExtractMeshDataPerSection(
        TArray<UStaticMeshComponent *> &components,
        TArray<MeshData> &meshdata
    );
    void ExtractMeshDataPerSection(UStaticMeshComponent *comp, TArray<MeshData> &meshdata);
    void ExtractMeshDataPerSection(UStaticMesh *comp, TArray<MeshData> &meshdata);
    
protected:
    void ExtractMeshDataPerSection(
        const FStaticMeshRenderData *renderdata,
        TArray<MeshData> &meshdataArray,
        int32 LODIndex
    );

public:
    //single meshdata copy
    void ExtractSingleMeshData(
        TArray<UStaticMeshComponent *> &components,
        MeshData &meshdata
    );

    void ExtractSingleMeshData(UStaticMeshComponent *comp, MeshData &meshData);
    
protected:
    void ExtractSingleMeshData(
        UStaticMesh *staticmesh,
        MeshData &singleMeshData,
        int32 LODIndex
    );

    void ExtractSingleMeshData(
        const FStaticMeshRenderData *renderdata,
        MeshData &singleMeshData,
        int32 LODIndex
    );

protected:
    FVector ToVectorDouble(FVector3f vertex);
};