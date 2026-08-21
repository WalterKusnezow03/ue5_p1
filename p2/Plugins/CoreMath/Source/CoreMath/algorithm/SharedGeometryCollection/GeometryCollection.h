#pragma once

#include "CoreMinimal.h"




// ---
// Shared Geometry Collection to have inside 
// poylgons, nodes and raw polygons to be added to pathfinder
// as nodes / and convex hulls
// ---


class COREMATH_API FGeometryCollection {

public:

    void Clear();

    TArray<FVector> &GetRawNodes();
    TArray<TArray<FVector>> &GetRawConvexHulls();

    void AddRawNodes(TArray<FVector> &array);

    void AddConvexHull(TArray<FVector> &array);

    void AddOffsetRawNodes(FVector offsetIn);

private:
    TArray<FVector> rawNodes;
    TArray<TArray<FVector>> rawConvexHulls;
    
};