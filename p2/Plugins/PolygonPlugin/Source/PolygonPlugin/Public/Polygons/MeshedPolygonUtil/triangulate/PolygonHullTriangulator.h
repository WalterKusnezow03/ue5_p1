#pragma once

#include "CoreMinimal.h"
#include "PolygonTriangle.h"

//traingulate via ear clipping
// --- badly tested - algorythm is BUGGED! ----
class POLYGONPLUGIN_API FPolygonHullTriangulator {

public:
    void Triangulate(const TArray<std::pair<int, int>> indices, bool clockwise);

    bool InsideHull(int x, int y) const;

    void Clear();

private:
    TArray<FPolygonTriangle> triangles;

    
};
