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
    
    void GetSubGraphPolygonMesh(const FVector &center, float sizeSquare, FMeshedPolygon &polygon);
    void GetSubGraphPolygonMesh(const FVector &a, const FVector &b, FMeshedPolygon &polygon);

    void debugShowAllNodes(UWorld *world);

    //with connect
    void addNode(PathFinderNode *node);
    void addAllNodes(std::vector<PathFinderNode *> &nodes);

    //from storage polygons
    void addAllPolygons(std::vector<FMeshedPolygon *> &polygon);
    void addPolygon(FMeshedPolygon *polygon);

    //adds new nodes based on hull and creates a tracked gridded polygon
    void addConvexHull(TArray<FVector> &vec);

    void addNewNode(FVector a);

    void addNoConnect(PathFinderNode *node);

    PathFinderNode *findNode(FVector node);
    PathFinderNode *findNodeInDirection(FVector &node, FVector &dir);

    std::vector<FMeshedPolygon *> GetAllPolygons();

protected:
    APathFinder *parent = nullptr;

    PathFinderQuadrant *TopRight = nullptr;
	PathFinderQuadrant *BottomRight = nullptr;
	PathFinderQuadrant *TopLeft = nullptr;
	PathFinderQuadrant *BottomLeft = nullptr;

	PathFinderQuadrant *askforQuadrant(int xIndex, int zIndex);
    PathFinderQuadrant *askforQuadrant(PathFinderNode *node);
    PathFinderQuadrant *askforQuadrant(const FVector &pos);

    TArray<PathFinderQuadrant *> allQuadrants();

    void GenerateRasterizedConvexHull(TArray<FVector> &polygon);
    float PolygonStepSize();
};