#pragma once

#include "CoreMinimal.h"

/// @brief class to store 2D Mesh Data for Slate Ui Polygons
/// Supports FVector2D and FSlateVertex at the same time.
class CUSTOMUIPLUGIN_API SlateMeshData {

    //SlateIndex = uin16, merken


public:
    SlateMeshData();
    ~SlateMeshData();

    /// @brief adds a triangle to the buffer
    void Append(FVector2D &a, FVector2D &b, FVector2D &c);

    ///@brief appends a triangle but looks for nearby vertecies
    void AppendEfficent(FVector2D &a, FVector2D &b, FVector2D &c);

    ///@brief appends a closed shape, is in clockwise order!
    void AppendClosedShape(TArray<FVector2D> &shape);


    TArray<FVector2D> &VerteciesRef();
    TArray<SlateIndex> &TrianglesRef();

    const TArray<FVector2D> &VerteciesRefConst() const;
    const TArray<FSlateVertex> MakeSlateVertexBuffer(FSlateRenderTransform &RenderTransform) const;
    const TArray<SlateIndex> &TrianglesRefConst() const;

    

private:
    float epsilon = 1.0f;

    int MaxSizeVertexBuffer = 65535; //numbers max for uint16
    TArray<FVector2D> Vertecies; //for line draw
    TArray<SlateIndex> Triangles;

    bool HasVertex(FVector2D &a, uint16 &outIndex);

    ///@brief may return -1
    int ClosestVertex(FVector2D &target);

    ///@brief may return -1
    int ClosestVertexBelowEpsilon(FVector2D &target);

    bool isValidVertexIndex(int i);

    bool TriangleCanBeAdded();

    FVector2D CenterOf(TArray<FVector2D> &buffer);

    FSlateVertex makeSlateVertex(
        const FVector2D &ref,
        const FSlateRenderTransform &RenderTransform
    ) const;

    //linear colors / gradient
    FLinearColor InterpolatedColorFor(const FVector2D &pos) const;
};
