#pragma once 

#include "CoreMinimal.h"
#include <map>

/// @brief tracks all intersections per road id, will sort them by index,
/// creating per road an intersection list.
class TERRAINPLUGIN_API RoadIntersectionMapped {

public:
    RoadIntersectionMapped();
    ~RoadIntersectionMapped();

    ///@brief add intersection to graph
    void Add(RoadIntersection &intersection);

    /// @brief will not allow to store new Intersections with add, graph will be locked.
    ///because adjacency is stored by ptr.
    void BuildGraph();

    void PrintGraphInfo();

    TArray<std::pair<FVector2D, FVector2D>> GetEdges();

private:
    bool bGraphLockedState = false;
    bool GraphIsLocked();
    void LockGraph();

    void BuildAdjacencyPerRoad();
    void BuildAdjacencyCrossed();
    void BuildAdjacencyCrossed(
        TArray<RoadIntersection> &intersections_i,
        TArray<RoadIntersection> &intersections_j
    );

    /// @brief intersections mapped by road id
    std::map<int, TArray<RoadIntersection>> intersectionsMapped;

    void AddAsA(RoadIntersection &intersection);
    void AddAsB(RoadIntersection &intersection);
    void Add(bool isA, RoadIntersection &intersection);




    std::map<int, TArray<RoadIntersection>>::iterator iteratorAt(int index);
};