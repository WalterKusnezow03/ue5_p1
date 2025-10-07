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



    //traversal and dissamble
    RoadIntersection* TraverseRightAndDisassembleEdge(
        RoadIntersection *prev,
        TArray<RoadIntersection *> &traversed
    );

    bool HasNeighbors();

    bool IsAdjacentSharedRoad(RoadIntersection &other, int &outid);
    bool IsAdjacentBothSharedRoad(RoadIntersection &other);

    ///@brief checks for same road ids and distance
    bool IsSameByDistance(RoadIntersection &other, int maxDistance);

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
    
    bool IsAdjacent(
        RoadIntersection &other,
        int &indexToRelink
    );

    bool HasLaterNode(
        RoadIntersection &other,
        RoadIntersection *&outPairBetween
    );





    bool ACloserThanB(
        int target,
        int A,
        int B
    );


//traversal / print
public:
    void AppendEdges(TArray<std::pair<FVector2D, FVector2D>> &edges);
    bool IsTraversed();

private:
    bool bTraversed = false;


//traversal disassembly
    bool ValidForTraversal(
        RoadIntersection *prev,
        TArray<RoadIntersection *> &traversed,
        RoadIntersection *compare
    );

    
};