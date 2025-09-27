#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "ProceduralMeshComponentCustom.generated.h"

/// @brief doesnt help.
UCLASS()
class GAMECORE_API UProceduralMeshComponentCustom : public UProceduralMeshComponent {

    GENERATED_BODY()

public:
    void UpdateMesh(int32 sectionIndex, MeshData &dataIn, bool bCreateCollision);

private:
    //fixed size vertex buffer which is allowed. Size never changed but unused vertecies moved to a single location
    int32 MaxSizeVertexBuffer = 300000;

    void UpdateBuffer(
        FProcMeshSection &section,
        const TArray<FVector> &Vertices,
        const TArray<FVector> &Normals,
        const TArray<FVector2D> &UV0,
        const TArray<FColor> &VertexColors,
        const TArray<FProcMeshTangent> &Tangents
    );

    void UpdateBuffer(
        TArray<FProcMeshVertex> &internalBuffer,
        const TArray<FVector> &Vertices,
        const TArray<FVector> &Normals,
        const TArray<FVector2D> &UV0,
        const TArray<FColor> &VertexColors,
        const TArray<FProcMeshTangent> &Tangents,
        int32 fromIndex,
        int32 toIndex,
        TArray<TFuture<void>> &Futures
    );

    void UpdateBoundsForSection(
        FProcMeshSection &section,
        const TArray<FVector> &Vertices
    );

    bool ValidTriangle(
        int32 v0, int32 v1, int32 v2, int32 sizeBuffer
    );
};
