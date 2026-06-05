#pragma once

#include "CoreMinimal.h"

//store the polygon edge indices explictly
class POLYGONPLUGIN_API FMeshedPolygonHullIndices{

public:
    void Clear();

    //FMeshedPolygonHullIndices
    void AddPosition(int x, int y);

    //moves all indices with a offset
    void Move(int x, int y);


    bool RayIntersectPolygon(
        int x, 
        int y, 
        const FVector2D &dir,
        FIntPoint &outClosestHit,
        float &outT
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

    int32 Num();
    std::pair<int, int>& operator[](int i);

    bool InsideHull(int x, int y) const;

protected:
    
    TArray<std::pair<int, int>> indices;

    
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
        const FVector2D &dir,
        int iStartEdge,
        int jEndEdge,
        FVector2D &outHit,
        float &outT
    );

    int Dot(std::pair<int, int> &n, int x, int y) const;

    std::pair<int, int> Normal(
        int i, int j
    ) const;
    std::pair<int, int> Connect(
        int i, int j) const;

    std::pair<int, int> Connect(
        const std::pair<int, int> &a,
        const std::pair<int, int> &b
    ) const ;
};