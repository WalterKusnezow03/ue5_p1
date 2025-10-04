#pragma once 

#include "CoreMinimal.h"
#include <map>
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/PolygonConstruction/RoadSectionList.h"

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


    //debug
    void PrintGraphInfo();
    TArray<std::pair<FVector2D, FVector2D>> GetEdges();

    TArray<FRoadSectionList>& DisassembleTraverseGraph();

private:
    void DisassembleTraverseGraphFrom(RoadIntersection *start);
    FRoadSectionList BuildCirlce(TArray<RoadIntersection *> &list);
    TArray<FRoadSectionList> dissassembledSections;

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
    void AddAllNodesToLinearListAfterBuild();

    /// @brief intersections mapped by road id
    std::map<int, TArray<RoadIntersection>> intersectionsMapped;
    TArray<RoadIntersection *> AllIntersections;

    void AddAsA(RoadIntersection &intersection);
    void AddAsB(RoadIntersection &intersection);
    void Add(bool isA, RoadIntersection &intersection);




    std::map<int, TArray<RoadIntersection>>::iterator iteratorAt(int index);
};