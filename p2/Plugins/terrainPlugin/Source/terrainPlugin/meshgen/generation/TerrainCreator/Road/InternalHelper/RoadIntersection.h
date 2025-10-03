#pragma once

#include "CoreMinimal.h"

/// @brief two road intersection tracker
class TERRAINPLUGIN_API RoadIntersection {

public:
    RoadIntersection();
    ~RoadIntersection();

    RoadIntersection(const RoadIntersection &other);
    RoadIntersection &operator=(const RoadIntersection &other);

    FString ToString();

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
    void ConnectBiDirectionalIfIsAdjacentAndRelinkNodes(RoadIntersection &other);




    ///@brief check once intersection map completed, 
    ///reference will be saved as ptr!,
    ///added neighbor added vice verca if adjacent, bi directional edge
    void ConnectBiDirectionalIfIsAdjacent(RoadIntersection &other);

    ///@brief check once intersection map completed, 
    ///reference will be saved as ptr!,
    ///added neighbor to this node only, not other. Single direction edge.
    bool AddAsNeighborIfIsAdjacent(RoadIntersection &other);

    ///@brief only call from RoadIntersectionMapped to add linear
    ///relation nodes
    bool AddNeighbor(RoadIntersection &other);

    ///@brief tries to remove a neighbor, if not found nothing happens
    void RemoveNeighbor(RoadIntersection &other);

private:
    int roadIdA;
    int roadIdB;

    int indexA();
    int indexB();

    std::pair<int, int> indexPairOfIntersection;
    FVector2D location;

    bool HasNeighbor(RoadIntersection &other);
    TArray<RoadIntersection *> adjacentNeighbors;



    bool IsAdjacent(RoadIntersection &other);
    bool IsAdjacentSharedRoad(RoadIntersection &other, int &outid);
    bool IsAdjacent(
        RoadIntersection &other,
        int &indexToRelink
    );

    bool HasLaterNode(
        RoadIntersection &other,
        RoadIntersection *&outPairBetween
    );

    ///@brief finds all shared neighbors with the shared road and sorts them by spline index
    TArray<std::pair<RoadIntersection *, int>> SortNeighborsByIndexSharedThisRoad(
        RoadIntersection &other
    );


//traversal / print
public:
    void AppendEdges(TArray<std::pair<FVector2D, FVector2D>> &edges);
    bool IsTraversed();

private:
    bool bTraversed = false;




};