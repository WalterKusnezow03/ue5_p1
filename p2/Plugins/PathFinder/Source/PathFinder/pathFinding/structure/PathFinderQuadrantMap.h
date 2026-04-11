#pragma once

#include "CoreMinimal.h"
#include "PathFinderQuadrant.h"
#include "PathFinderNode.h"

class APathFinder;

class PATHFINDER_API PathFinderQuadrantMap{

public:
    PathFinderQuadrantMap(APathFinder *finder);
    ~PathFinderQuadrantMap();

    void clear();

    std::vector<PathFinderNode *> getSubGraph(FVector a, FVector b);

    void debugShowAllNodes(UWorld *world);

    void addNode(PathFinderNode *node);
    void addNewNode(FVector a);

    void addNoConnect(PathFinderNode *node);

    PathFinderNode *findNode(FVector node);
    PathFinderNode *findNodeInDirection(FVector &node, FVector &dir);

protected:
    APathFinder *parent = nullptr;

    PathFinderQuadrant *TopRight = nullptr;
	PathFinderQuadrant *BottomRight = nullptr;
	PathFinderQuadrant *TopLeft = nullptr;
	PathFinderQuadrant *BottomLeft = nullptr;

	PathFinderQuadrant *askforQuadrant(int xIndex, int zIndex);
    PathFinderQuadrant *askforQuadrant(PathFinderNode *node);
};