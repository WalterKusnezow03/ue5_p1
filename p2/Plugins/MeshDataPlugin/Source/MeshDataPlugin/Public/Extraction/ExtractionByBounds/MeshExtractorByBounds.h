// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "CoreMath/Matrix/MMatrix.h"
#include "MeshDataPlugin/Public/Extraction/StaticMeshCopy/MeshExtractorBase.h"

class MESHDATAPLUGIN_API MeshExtractorByBounds {
private:
    float depthMax = 100.0f;

public:
    void SetMaxDepth(float depthIn){
        depthMax = std::abs(depthMax);
    }

    void ExtractSingleMeshDataByBounds(
        UStaticMeshComponent *componentToExtractFrom, 
        MeshData &meshData,
        UStaticMeshComponent *boundsComponent //must be attached inside component to extract from(!)
    );

    void ExtractSingleMeshDataByBounds(
        UStaticMeshComponent *componentToExtractFrom,
        MeshData &meshData,
        UStaticMeshComponent *boundsComponent,
        FVector offset
    );
private:
    void RotateOffset(
        UStaticMeshComponent *component,
        FVector &offset
    );

    MMatrix GenerateRelativeTransform(
        UStaticMeshComponent *component
    );
    MMatrix GenerateInverseRelativeTransform(
        UStaticMeshComponent *component
    );
    FVector2D ExtractBounds2D(UStaticMeshComponent *comp);

    void CutAgainstBounds(UStaticMeshComponent *comp, MeshData &projectedData);
    void CutAgainstBounds(const FVector2D &bounds, MeshData &projectedData);

    bool IsInBound(
        const FVector2D &halfBound,
        int32 i0,
        int32 i1,
        int32 i2,
        TArray<FVector> &vertexBuffer
    );
    bool IsInBound(
        const FVector2D &halfBound,
        const FVector &vertex
    );
};