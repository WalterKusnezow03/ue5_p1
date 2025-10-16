#pragma once

#include "CoreMinimal.h"

#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/IntersectionGraph.h"

class TERRAINPLUGIN_API RoadData {

public:
    RoadData();
    RoadData(int id, const TVector<FVector2D> &curveIn);
    RoadData(const RoadData &other);
    RoadData &operator=(const RoadData &other);
    ~RoadData();

    int Id();

    //new
    void FindIntersections(RoadData &other, IntersectionGraph &outIntersections);

    



    std::vector<FVector2D> getCurve();
    TArray<FVector2D> getCurveAsTArray();

private:
    int roadId;
    TVector<FVector2D> curve;

    void FindIntersections(
        TVector<FVector2D> &curveA,
        TVector<FVector2D> &curveB,
        int idOther,
        IntersectionGraph &outIntersections
    );

    bool DoesIntersect(
        const FVector2D &v0,
        const FVector2D &v1,
        const FVector2D &d0,
        const FVector2D &d1,
        FVector &IntersectionPoint
    );
};