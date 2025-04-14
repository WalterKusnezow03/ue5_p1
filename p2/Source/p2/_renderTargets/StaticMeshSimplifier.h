#pragma once

#include "CoreMinimal.h"

class P2_API StaticMeshSimplifier {

public:
    StaticMeshSimplifier();
    ~StaticMeshSimplifier();

    void append(
        TArray<FVector3f> &vertexBuffer,
        TArray<int32> &trianglesIn
    );

    void appendEfficent(
        FVector3f &a,
        FVector3f &b,
        FVector3f &c);

private:
    float epsilon = 5.0f;
    TArray<FVector3f> vertecies;
    TArray<int32> triangles;

    int alreadyHasVertex(FVector3f &other);
    bool isValidVertexIndex(int index);
    bool isValidVertexIndex(int index, TArray<FVector3f> &verteciesIn);
    bool isValidVertexIndex(int v0, int v1, int v2, TArray<FVector3f> &verteciesIn);
};