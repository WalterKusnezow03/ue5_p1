#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/MeshedPolygonHullIndices.h"

class POLYGONPLUGIN_API FMeshedPolygonHullSet {

public:

    void AppendAsBinary(
        TArray<uint8> &buffer
    );
    void LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *&Ptr // reference to a pointer. Pointer by reference.
    );

    void Add(FMeshedPolygonHullIndices &hull);

    FMeshedPolygonHullIndices &operator[](int32 i);
    int32 Num(){
        return set.Num();
    }

    //copies the set but adds an offset to the added hulls
    void AddSetWithOffset(const FMeshedPolygonHullSet &other, int offsetX, int offsetY);

    bool RayIntersectPolygons(
        int x, 
        int y, 
        const FVector2D &dir,
        FIntPoint &outClosestHit
    );

    bool RayIntersectPolygons(
        int x, 
        int y, 
        const FVector2D &dir,
        FIntPoint &outClosestHit,
        float &outT
    );

    void Clear();

    int NumEdges();

protected:
    TArray<FMeshedPolygonHullIndices> set;
};
