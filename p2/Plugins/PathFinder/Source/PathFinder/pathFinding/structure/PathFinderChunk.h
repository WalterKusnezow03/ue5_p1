#pragma once


#include "CoreMinimal.h"
#include "PathFinderNode.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

class APathFinder;

class PATHFINDER_API PathFinderChunk{

private:
    /// @brief is a vector of pointers in case the vector is copied internally
    /// and nodes must stay active while path finding
    std::vector<PathFinderNode *> nodes;

    // saving polygons gridded per aabb (call it sparse grid if you want to, 
    // no real full occupancy grid)
    std::vector<FMeshedPolygon *> polygons;

public:
    PathFinderChunk(APathFinder *parentIn);
    ~PathFinderChunk();

    

    void add(FVector vec);
    void add(PathFinderNode *node);
    void add(FMeshedPolygon *polygon);

    //add a node without connecting it.
    //no distance check, just add!
    //designed to be called when using storage
    void addNoConnect(PathFinderNode *node);

    std::vector<PathFinderNode *> &getNodes();
    PathFinderNode *findNode(FVector pos);
    PathFinderNode *findNodeInDirection(FVector &node, FVector &dir);

    std::vector<FMeshedPolygon *> &getPolygons();

    bool hasNode(FVector pos);
    

    void clear();

    PathFinderNode *lateadd(FVector pos);

    void debugShowAllNodes(UWorld *world);

    //appends all owned ploygons by this chunk
    void AppendAllPolygons(std::vector<FMeshedPolygon *> &polygonsOut);

    // new:
    //std::vector<PathFinder::ConvexPolygon *> polygons;

private:
    APathFinder *parent = nullptr;
};