#pragma once 

#include "CoreMinimal.h"

class TERRAINPLUGIN_API TwoRoadIntersection {

public:
    bool operator==(const TwoRoadIntersection &other);

    void Setup(
        int roadIdAIn, 
        int roadIdBIn, 
        const std::pair<int, int> &indexPair,
        FVector2D &locationIn
    );

    int RoadIdA();
    int RoadIdB();
    int indexA();
    int indexB();
    int IndexForRoadId(int someRoadId);

    bool SharesSameRoadAndIndex(TwoRoadIntersection &other);

   
    bool IsAdjacentSharedRoad(TwoRoadIntersection &other, int &outId);

    bool IsSame(TwoRoadIntersection &other);

    FVector2D getLocation() const {
        return location;
    }

private:

    int roadIdA = 0;
    int roadIdB = 0;
    std::pair<int,int> indexPairOfIntersection;
    FVector2D location;


};