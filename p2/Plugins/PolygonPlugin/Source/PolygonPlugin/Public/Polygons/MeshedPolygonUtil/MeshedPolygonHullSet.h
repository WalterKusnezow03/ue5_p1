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

    
    bool IsVisible(const FVector2D &a, const FVector2D &b);

    bool IsVisibleHull(const TArray<FVector2D> &array);

    /// ----- Eject self cutting polygons on outer hull-----
    /// -- ear clipping like algorythm to cut loops --
    void SplitSelfCuttingPolygonsFromOuterHull();

    void SortByArea();

protected:
    bool IsVisble(int x, int y, int xB, int yB);

    TArray<FMeshedPolygonHullIndices> set;

    void AddAll(TArray<FMeshedPolygonHullIndices> &hulls);
    void SplitSelfCuttingPolygonsFromHullAt(int i);

    bool RayIntersectPolygonUpdateT(
        int x,
        int y,
        const FVector2D &dir,
        FIntPoint &outClosestHit,
        float &outT,
        int layer
    );
};
