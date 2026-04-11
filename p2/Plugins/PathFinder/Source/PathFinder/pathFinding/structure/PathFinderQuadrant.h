#pragma once

#include "CoreMinimal.h"

class APathFinder;

class PATHFINDER_API PathFinderQuadrant{
private:
    int xSample;
    int ySample;

    APathFinder *parent = nullptr;

public:
    std::vector<std::vector<PathFinderChunk*>> map;
    PathFinderQuadrant(int xSampleIn, int zSampleIn, APathFinder* parentIn);
    ~PathFinderQuadrant();

    PathFinderNode *findNode(FVector pos);
    PathFinderNode *findNodeInDirection(FVector &node, FVector &dir);
    std::vector<PathFinderNode *> nodesEnClosedBy(float xA, float zA, float xB, float zB);

    std::vector<PathFinderNode *> askForArea(FVector a, FVector b);

    void add(FVector n);
    void add(PathFinderNode *node);

    //add a new node without connection
    //designed to be called when using storage
    void addNoConnect(PathFinderNode *node);

    void clear();

    void fillMapTo(int xIndex, int yIndex);

    void debugShowAllNodes(UWorld *world);

    int chunkCount();
};