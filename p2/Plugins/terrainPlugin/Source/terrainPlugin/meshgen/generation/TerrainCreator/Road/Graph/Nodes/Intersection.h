#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/TwoRoadIntersection.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/PolygonConstruction/RoadSectionList.h"

//new construction and deconstruction via edges
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/Edge/Edge.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/TraversalResult/IntersectionTraversalResult.h"

class TERRAINPLUGIN_API Intersection {

public:
    Intersection();
    ~Intersection();
    Intersection(const Intersection &other);
    Intersection &operator=(const Intersection &other);
    Intersection(const TwoRoadIntersection &other);

    //tries to join a two road intersection
    bool Join(TwoRoadIntersection &other);

    //build an edge.
    void AddAsNeighborIfAdjacent(Intersection *ptr);



    Intersection* TraverseRightAndDisassembleEdge(
        Intersection *prev,
        Intersection *targetedEnd,
        FRoadSectionList &sectionListAppend
    );

    ///adds all visiual edges to the neighbors to the outArray
    void AppendEdges(TArray<std::pair<FVector2D, FVector2D>> &outEdges);

    bool HasNeighbors();



    //not tested
    void PostCleanUpAdjacency();

private:
    

    bool HasNeighbor(Intersection &other);
    bool HasNeighbor(Edge &other);
    
    // void AddNeighbor(Intersection *other);
    // void RemoveNeighbor(Intersection *other);
    bool RemoveNeighbor(Edge *other);
    bool RemoveEdgeAtIndex(int i);
    // void RemoveAndAddNeighbor(Intersection *remove, Intersection *add);

    bool HasSame(TwoRoadIntersection &other);
    bool CanJoin(TwoRoadIntersection &compare);

    bool AIsCloser(int target, int a, int b);

    
    
    bool IsAdjacent(
        Intersection &other,
        TArray<Edge> &outEdges,
        TArray<Edge*> &outMarkedForRemoval
    );

    int IntersectionIndexFor(int roadId);
    TArray<Edge *> IntersectionsToRemove(
        int roadId,
        int indexOwn,
        int otherIndex
    );

    void AppendSharedRoadSection(
        Edge *someNeighbor,
        FRoadSectionList &sectionListAppend
    );

    
    
    //will be one anyways.
    TArray<TwoRoadIntersection> intersectionsInternal;
    //TArray<Intersection *> neighbors;

    //new
    TArray<Edge> edgesToNeighbors;

    FVector2D location;

    //void RemoveAll(TArray<Intersection *> &others);
    //void AddAllNeighbors(TArray<Intersection *> &others);
    //void AddNeighborToAll(TArray<Intersection *> &others, Intersection *neighbor);
    //void RemoveNeighborFromAll(TArray<Intersection *> &others, Intersection *neighbor);
    void RemoveNeighborFromAll(TArray<Edge *> &others, Intersection *neighbor);
    void RemoveAllEdgesWithNeighbor(Intersection *neighbor);

    void AddAllNeighbors(TArray<Edge> &others);
    void AddAllNeighbors(TArray<Edge*> &others);

    void RemoveAll(TArray<Edge> &others);
    void RemoveAll(TArray<Edge *> &others);
    void AddNeighborToAll(
        TArray<Edge *> &othersmarkedForRemoval,
        TArray<Edge> &edgesMade
    );
    void AddNeighbor(Edge &other);

    float Dot2D(FVector2D &a, FVector2D &b);



    //traversal
    
    /// @brief most right off test / in dir of normal for all edges and anti paralell to passed
    /// dir from prev.
    /// @param prev 
    /// @param normal 
    /// @param dirFromPrev 
    /// @return 
    IntersectionTraversalResult FindMostRightOff(
        Intersection *prev,
        Intersection *targetedEnd,
        FVector2D &normal,
        FVector2D &dirFromPrev
    );

    ///@brief checks if the node has enough neighbors.
    bool ValidForTraversal(
        Intersection *intersection
    );

    bool IsPrev(Intersection *a, Intersection *b);

    ///will process the result and flagged was valid.
    bool ProcessResult(
        IntersectionTraversalResult &result,
        FRoadSectionList &sectionListAppend //append to read section (traversed edge, bspline indices.)
    );
};