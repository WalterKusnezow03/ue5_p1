#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/TwoRoadIntersection.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/PolygonConstruction/RoadSectionList.h"

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
        TArray<Intersection *> &traversed,
        FRoadSectionList &sectionListAppend
    );

    void AppendEdges(TArray<std::pair<FVector2D, FVector2D>> &outEdges);

    bool HasNeighbors();


private:
    bool HasNeighbor(Intersection *other);
    void AddNeighbor(Intersection *other);
    void RemoveNeighbor(Intersection *other);
    void RemoveAndAddNeighbor(Intersection *remove, Intersection *add);

    bool HasSame(TwoRoadIntersection &other);
    bool CanJoin(TwoRoadIntersection &compare);

    bool AIsCloser(int target, int a, int b);

    bool IsAdjacent(Intersection &other, int &outRoadId);
    int IntersectionIndexFor(int roadId);
    TArray<Intersection *> IntersectionsToRemove(
        int roadId,
        int indexOwn,
        int otherIndex
    );

    void AppendSharedRoadSection(
        Intersection *someNeighbor,
        FRoadSectionList &sectionListAppend
    );

    TArray<TwoRoadIntersection> intersectionsInternal;

    TArray<Intersection *> neighbors;

    FVector2D location;

    void RemoveAll(TArray<Intersection *> &others);
    void AddAllNeighbors(TArray<Intersection *> &others);
    void AddNeighborToAll(TArray<Intersection *> &others, Intersection *neighbor);
    void RemoveNeighborFromAll(TArray<Intersection *> &others, Intersection *neighbor);

    float Dot2D(FVector2D &a, FVector2D &b);

    bool ValidForTraversal(
        Intersection *prev,
        TArray<Intersection *> &traversed,
        Intersection *compare
    );
};