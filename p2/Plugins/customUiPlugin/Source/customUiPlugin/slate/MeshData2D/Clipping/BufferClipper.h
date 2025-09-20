#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/MeshDataWrap/SlateMeshDataPolygon.h"
#include "FClipEdge.h"

/// @brief not tested at all.
class CUSTOMUIPLUGIN_API BufferClipper {

public:
    static SlateMeshDataPolygon Test();

    //will feature double vertecies.
    static void ClipBuffer(
        const TArray<FVector2D> &buffer,
        const TArray<int32> &triangles,
        FVector2D &topLeft,
        FVector2D &bottomRight,
        TArray<FVector2D> &outBuffer, //must be empty, outBuffer Triangle Shaped!
        TArray<int32> &outTriangles //must be empty
    );

private:
    static TArray<FClipEdge> MakeEdgesFromQuad(
        const FVector2D &topLeft,
        const FVector2D &bottomRight
    );

    static FClipEdge MakeEdge(
        const FVector2D &a,
        const FVector2D &b
    );

    static void ClipOneInside(
        FVector2D &v0_inside,
        FVector2D &v1,
        FVector2D &v2,
        FClipEdge &edge,
        TArray<FVector2D> &appendBuffer
    );

    static void ClipTwoInside(
        FVector2D &v0_inside,
        FVector2D &v1,
        FVector2D &v2_inside,
        FClipEdge &edge,
        TArray<FVector2D> &appendBuffer
    );

    static void GenerateTriangleBuffer(
        TArray<FVector2D> &vertxBufferTriangleShaped,
        TArray<int32> &outTriangles
    );
};