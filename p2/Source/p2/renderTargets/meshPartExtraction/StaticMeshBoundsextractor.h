#pragma once

#include "CoreMinimal.h"

class P2_API StaticMeshBoundsextractor {

public:
    StaticMeshBoundsextractor();
    ~StaticMeshBoundsextractor();

    void CopyInBounds(
        FVector2D &a,
        FVector2D &b,
        FVector &normal,
        TArray<FVector3f> &verteciesIn,
        TArray<int32> &trianglesIn,
        FVector paneLocalLocation,
        FVector actorLocation,
        UWorld *worldIn
    );

    

private:
    UWorld *world;

    void FindInBounds(
        FVector2D &a,
        FVector2D &b,
        TArray<FVector> &verteciesIn,
        TArray<int32> &verteciesOut,
        FVector rotatedNormal
    );

    TArray<FVector> vertecies;
    TArray<int32> triangles;

    void copy(
        TArray<FVector> &rawBuffer,
        TArray<int32> &inBoundsVertecies,
        TArray<int32> &trianglesIn,
        FVector actorLocation
    );

    bool isInBounds(
        FVector2D &a,
        FVector2D &b,
        FVector &vertex
    );

    
};