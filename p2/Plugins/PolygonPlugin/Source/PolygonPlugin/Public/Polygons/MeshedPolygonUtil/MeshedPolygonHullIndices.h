#pragma once

#include "CoreMinimal.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/triangulate/PolygonHullTriangulator.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/HitResult/MeshedPolygonHitTask.h"

//store the polygon edge indices explictly
class POLYGONPLUGIN_API FMeshedPolygonHullIndices{

public:
    void Clear();

    //FMeshedPolygonHullIndices
    void AddPosition(const FVector2D &pos);
    void AddPosition(const FVector &pos);
    void AddPosition(int x, int y);
    void AddPosition(const std::pair<int,int> &pos);

    //moves all indices with a offset
    void Move(int x, int y);


    bool RayIntersectPolygon(
        int x, 
        int y, 
        const FVector2D &dir,
        FIntPoint &outClosestHit,
        float &outT
    );

    bool RayIntersectPolygon(
        int x, 
        int y,
        FMeshedPolygonHitTask &result
    );

    bool RayIntersectPolygonFarHit(
        int x, 
        int y, 
        const FVector2D &dir,
        FIntPoint &outClosestHit,
        float &outT
    );

    void AppendAsBinary(
        TArray<uint8> &buffer
    );
    void LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *&Ptr // reference to a pointer. Pointer by reference.
    );

    int32 Num() const;
    std::pair<int, int>& operator[](int i);
    FVector Get(int i);

    void Triangulate(bool clockwise);

    //triangulation test - bad - do not use
    bool InsideHull(int x, int y) const;

    void AddAll(const TArray<FIntPoint> &hits);
    void AddAll(const TArray<FVector2D> &points);

    bool EdgeDirection(int i, int j, FVector2D &outDir);
    float EdgeAngleDot(int i);

    //clean up with graham scan: convex hull
    void ConvertToConvexHull();

    void EjectSelfCuttingPolygons(TArray<FMeshedPolygonHullIndices> &outArray);
    
    //area of polygon - for non intersecting, counter clockwise
    float Area() const;

    //num hits test - better than ear clipping
    bool IsInside(const TArray<FVector2D> &array, FVector &notInside);
    bool IsInside(const FVector2D &pos);
    bool IsInside(int x, int y);

    //checks if all vertecies can see each other in a row
    bool IsVisibleHull(const TArray<FVector2D> &array);
    bool IsVisibleHull(const TArray<FVector2D> &array, FVector &outHit);

    bool CanSee(const FVector2D &a, const FVector2D &b, FVector &outhit);

protected:
    //saved to storage
    TArray<std::pair<int, int>> indices;

    //not saved to storage
    FPolygonHullTriangulator triangulator;

    float Dot(const FVector2D &a, const FVector2D &b);


    bool RayIntersectAt(
        const FVector2D &v,
        const FVector2D &dir,
        const FVector2D &e0,
        const FVector2D &e1,
        FVector2D &outHit,
        float &outT
    );

    bool RayIntersect(
        int x, 
        int y, 
        int iStartEdge,
        int jEndEdge,
        FMeshedPolygonHitTask &result
    );

    bool RayIntersectAt(
        const FVector2D &v, // vertex start
        const FVector2D &e0,
        const FVector2D &e1,
        FMeshedPolygonHitTask &result
    );

    int Dot(std::pair<int, int> &n, int x, int y) const;

    FVector2D Dir(const std::pair<int, int> &a, const std::pair<int, int> &b);
    bool DirValid(
        const std::pair<int, int> &a, 
        const std::pair<int, int> &b,
        FVector2D &outDir
    );

    int SafeIndex(int i);

    

    //internal intersect tests
    bool IsVisble(
        std::pair<int, int> &posA,
        std::pair<int, int> &posB,
        int &outHitEdgeIndex,
        int ignoreEdge
    );
    bool IsVisble(int x, int y, int xB, int yB, int &outHitEdgeIndex, int ignoreEdge);

    void AppendRange(FMeshedPolygonHullIndices &outPolygon, int i, int j);

    void RemoveAll(const TArray<FMeshedPolygonHullIndices> &array);
    void Remove(const FMeshedPolygonHullIndices &removeIndices);
};