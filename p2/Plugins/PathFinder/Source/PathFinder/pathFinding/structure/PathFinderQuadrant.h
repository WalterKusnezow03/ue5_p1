#pragma once

#include "CoreMinimal.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

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
    
    std::vector<PathFinderNode *> askForArea(FVector a, FVector b);
    std::vector<FMeshedPolygon *> getPolygonsInArea(FVector a, FVector b);

    void add(FVector n);
    void add(PathFinderNode *node);
    void add(FMeshedPolygon *polygon);

    //add a new node without connection
    //designed to be called when using storage
    void addNoConnect(PathFinderNode *node);

    void clear();

protected:
    std::vector<PathFinderNode *> nodesEnClosedBy(float xA, float zA, float xB, float zB);
    std::vector<FMeshedPolygon *> polygonsEnClosedBy(float xA, float yA, float xB, float yB);

protected:
    void fillMapTo(int xIndex, int yIndex);
    void fillMapToPosition(PathFinderNode *node);
    void fillMapToPosition(const FVector &pos);

    bool MakeIndex(const FVector &pos, int &x, int &y);


    TArray<PathFinderChunk*> ChunksEnclosedBy(float xA, float yA, float xB, float yB);

public:
    void debugShowAllNodes(UWorld *world);

    int chunkCount();

    //generates index boundign box clamped against quadrant location on xy plane
    //clamped.
    void MakeCollectBoundingBoxClamped(
        const FVector &a,
        const FVector &b,
        float &lowerX,
        float &lowerY,
        float &higherX,
        float &higherY
    );

    
    void AppendAllPolygons(std::vector<FMeshedPolygon *> &polygonsOut);
};