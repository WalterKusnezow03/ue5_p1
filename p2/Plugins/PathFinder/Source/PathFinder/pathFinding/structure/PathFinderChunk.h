#pragma once


#include "CoreMinimal.h"
#include "PathFinderNode.h"

class APathFinder;

class PATHFINDER_API PathFinderChunk{
public:
    PathFinderChunk(APathFinder *parentIn);
    ~PathFinderChunk();

    /// @brief is a vector of pointers in case the vector is copied internally
    /// and nodes must stay active while path finding
    std::vector<PathFinderNode *> nodes;

    void add(FVector vec);
    void add(PathFinderNode *node);

    //add a node without connecting it.
    //designed to be called when using storage
    void addNoConnect(PathFinderNode *node);

    std::vector<PathFinderNode *> &getNodes();
    PathFinderNode *findNode(FVector pos);
    PathFinderNode *findNodeInDirection(FVector &node, FVector &dir);

    bool hasNode(FVector pos);
    

    void clear();

    PathFinderNode *lateadd(FVector pos);

    void debugShowAllNodes(UWorld *world);

    // new:
    //std::vector<PathFinder::ConvexPolygon *> polygons;

private:
    APathFinder *parent = nullptr;
};