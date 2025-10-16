#pragma once

#include "CoreMinimal.h"

class Intersection;

class TERRAINPLUGIN_API Edge {

public:
    Edge();
    ~Edge();
    Edge(const Edge &other);
    Edge &operator=(const Edge &other);

    void Setup(Intersection *node, TwoRoadIntersection &a, TwoRoadIntersection &b);

    //update for edge copy: change node bi directional edge
    void ChangeIntersection(Intersection *node);

    Intersection *getIntersection();

    bool IsValidEgde();
    FVector2D location();

    int IntersectionIndexA(int roadId);
    int IntersectionIndexB(int roadId);

    bool IsSame(Edge *other);

    void AppendEdge(
        TArray<std::pair<FVector2D, FVector2D>> &outEdges
    );

    TwoRoadIntersection &getConnectedA(){
        return connectedA;
    }
    TwoRoadIntersection &getConnectedB(){
        return connectedB;
    }


    ///@brief checks if the other edeg is closed up by bounds of bspline
    bool OtherIsCloser(Edge &other);
    bool SharedRoad(Edge &other, int &outId);

private:
    std::pair<int, int> IndexPairConnectedABForRoadId(int roadId);
    bool isBetween(int lower, int higher, int lowerOther, int higherOther);

    Intersection *connectedNode = nullptr;

    //reason to save an edge and remove later to prevent
    //traversal 
    TwoRoadIntersection connectedA;
    TwoRoadIntersection connectedB;
};