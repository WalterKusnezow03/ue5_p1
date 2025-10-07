#pragma once 

#include "CoreMinimal.h"
#include <map>
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/TwoRoadIntersection.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/Intersection.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/PolygonConstruction/RoadSectionList.h"




/// @brief tracks all intersections per road id, will sort them by index,
/// creating per road an intersection list.
class TERRAINPLUGIN_API IntersectionGraph {

public:
    IntersectionGraph();
    ~IntersectionGraph();

    

    ///@brief add intersection to graph
    void Add(TwoRoadIntersection &intersection);

    /// @brief will not allow to store new Intersections with add, graph will be locked.
    ///because adjacency is stored by ptr.
    void BuildGraph();


    //debug
    TArray<std::pair<FVector2D, FVector2D>> GetEdges();

    TArray<FRoadSectionList>& DisassembleTraverseGraph();

private:
    void DisassembleTraverseGraphFrom(Intersection *start);
    
    TArray<FRoadSectionList> dissassembledSections;

private:
    bool bGraphLockedState = false;
    bool GraphIsLocked();
    void LockGraph();

    
    void BuildAdjacencyCrossed();
    

    TArray<Intersection> nodes;
};