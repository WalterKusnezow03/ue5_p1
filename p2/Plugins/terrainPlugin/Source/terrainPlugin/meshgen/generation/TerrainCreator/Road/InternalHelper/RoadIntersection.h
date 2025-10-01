#pragma once

#include "CoreMinimal.h"

/// @brief two road intersection tracker
class TERRAINPLUGIN_API RoadIntersection {

public:
    RoadIntersection();
    ~RoadIntersection();

    RoadIntersection(const RoadIntersection &other);
    RoadIntersection &operator=(const RoadIntersection &other);

    void Setup(
        int roadIdAIn, 
        int roadIdBIn, 
        const std::pair<int, int> &indexPair,
        FVector2D &locationIn
    );

    int RoadIdA();
    int RoadIdB();


    int IndexForRoadId(int someRoadId);


    ///@brief check once intersection map completed, 
    ///reference will be saved as ptr!
    void AddAsNeighborIfIsAdjacent(RoadIntersection &other);

    ///@brief only call from RoadIntersectionMapped to add linear
    ///relation nodes
    void AddNeighbor(RoadIntersection &other);

private:
    int roadIdA;
    int roadIdB;

    std::pair<int, int> indexPairOfIntersection;
    FVector2D location;

    bool HasNeighbor(RoadIntersection &other);
    TArray<RoadIntersection *> adjacentNeighbors;
};