#include "RoadIntersectionMapped.h"
#include "GameCore/DebugHelper.h"


RoadIntersectionMapped::RoadIntersectionMapped(){
    bGraphLockedState = false;
}
RoadIntersectionMapped::~RoadIntersectionMapped(){

}


void RoadIntersectionMapped::PrintGraphInfo(){
    FString prefix = TEXT("RoadIntersectionMapped::");
    for (auto &pair : intersectionsMapped)
    {
        int roadId = pair.first;
        int linearNeighbors = pair.second.Num();
        DebugHelper::logMessage(
            FString::Printf(TEXT("%s Road id(%d) intersections(%d)"), *prefix, roadId, linearNeighbors)
        );
    }
}






bool RoadIntersectionMapped::GraphIsLocked(){
    return bGraphLockedState;
}
void RoadIntersectionMapped::LockGraph(){
    bGraphLockedState = true;
}

void RoadIntersectionMapped::Add(RoadIntersection &intersection){
    if(HasIntersection(intersection)){
        DebugHelper::logMessage("RoadIntersectionMapped::Add Already Has Similar Intersection!");
        return;
    }

    //add per road id
    AddAsA(intersection); 
    AddAsB(intersection);
}

void RoadIntersectionMapped::AddAsA(RoadIntersection &intersection){
    Add(true, intersection);
}

void RoadIntersectionMapped::AddAsB(RoadIntersection &intersection){
    Add(false, intersection);
}

bool RoadIntersectionMapped::HasIntersection(RoadIntersection &intersection){
    return 
    HasIntersection(true, intersection) ||
    HasIntersection(false, intersection);
}

bool RoadIntersectionMapped::HasIntersection(bool isA, RoadIntersection &intersection){
    int id = isA ? intersection.RoadIdA() : intersection.RoadIdB();
    if(intersectionsMapped.find(id) != intersectionsMapped.end()){
        TArray<RoadIntersection> &intersectionsTracked = intersectionsMapped[id];
        for (int i = 0; i < intersectionsTracked.Num(); i++){
            RoadIntersection &current = intersectionsTracked[i];
            //bool RoadIntersection::IsSameByDistance(RoadIntersection &other, int maxDistance)

            int maxDist = terrainConstants::ONEMETER * 3;
            if (current.IsSameByDistance(intersection, maxDist)){
                return true;
            }
        }
    }
    return false;
}

void RoadIntersectionMapped::Add(bool isA, RoadIntersection &intersection){
    if(GraphIsLocked()){
        DebugHelper::logMessage("RoadIntersectionMapped::Intersction cant be added: graph locked!");
        return;
    }

    int id = isA ? intersection.RoadIdA() : intersection.RoadIdB();
    if(intersectionsMapped.find(id) == intersectionsMapped.end()){
        intersectionsMapped[id] = TArray<RoadIntersection>();
    }
    TArray<RoadIntersection> &intersectionsTracked = intersectionsMapped[id];

    ///own index of intersection to check to sort in properly
    int indexOfIntersection = intersection.IndexForRoadId(id);

    //sort in by index
    for (int i = 0; i < intersectionsTracked.Num(); i++){
        int next = i + 1; 
        if(next < intersectionsTracked.Num()){
            RoadIntersection &intersectionNext = intersectionsTracked[next];
            int index = intersectionNext.IndexForRoadId(id);
            if(index != -1){
                if(indexOfIntersection <= index){
                    //insertAt(i, before next)
                    intersectionsTracked.Insert(intersection, i); //right infront added, at the index as expected 
                    DebugHelper::logMessage("RoadIntersectionMapped::Intersction inserted!");
                    return;
                }
            }
        }
    }
    
    //fall through array because empty or very highest index: Add
    intersectionsTracked.Add(intersection);
    DebugHelper::logMessage("RoadIntersectionMapped::Intersction Appended!");


}


void RoadIntersectionMapped::BuildGraph(){
    LockGraph(); //doesnt allow adding new nodes: Array might resize, all ptrs 
    //stores as adjacent neighbors will be invalid.

    //O(n^4)
    BuildAdjacencyPerRoad(); //ordered linerally added one by one linked list like
    BuildAdjacencyCrossed(); //cross all roads which each other
    AddAllNodesToLinearListAfterBuild();
}

void RoadIntersectionMapped::BuildAdjacencyPerRoad(){
    //build linear Adjacency relaations O(n) per chain
    for(auto &pair : intersectionsMapped){
        TArray<RoadIntersection> &intersections = pair.second;
        //add liniearly from prev to this, ++, linked chain
        for (int i = 1; i < intersections.Num(); i++){
            RoadIntersection &prev = intersections[i - 1];
            RoadIntersection &current = intersections[i];
            prev.AddNeighbor(current);
        }
    }
}

void RoadIntersectionMapped::BuildAdjacencyCrossed(){
    //build Adjacency relations O(n^2) per list, O(n^4) for all roads with redundancy

    int i = 0;
    for (auto it_i = iteratorAt(0); it_i != intersectionsMapped.end(); ++it_i, i++)
    {
        auto &pair_i = *it_i;
        TArray<RoadIntersection> &intersections_i = pair_i.second;

        for (auto it_j = iteratorAt(i); it_j != intersectionsMapped.end(); ++it_j){
            auto &pair_j = *it_j;
            TArray<RoadIntersection> &intersections_j = pair_j.second;
            BuildAdjacencyCrossed(intersections_i, intersections_j);
        }
    }
}

void RoadIntersectionMapped::BuildAdjacencyCrossed(
    TArray<RoadIntersection> &intersections_i,
    TArray<RoadIntersection> &intersections_j
){
    //O(n^2)
    for (int i = 0; i < intersections_i.Num(); i++){
        RoadIntersection &intersectionCurrent_i = intersections_i[i];
        for (int j = i; j < intersections_j.Num(); j++)
        {
            RoadIntersection &intersectionCurrent_j = intersections_j[j];

            //relink connections which are deprecated if adjacent along a road.
            intersectionCurrent_i.ConnectBiDirectionalIfIsAdjacentAndRelinkNodes(intersectionCurrent_j);
        }
    }
}

/// @brief not sure if this okay. The map is not unordered, some sort of tree inside.
std::map<int, TArray<RoadIntersection>>::iterator RoadIntersectionMapped::iteratorAt(int index){
    if(index >= 0 && index < intersectionsMapped.size()){
        return std::next(intersectionsMapped.begin(), index);
    }
    return intersectionsMapped.begin();
}

void RoadIntersectionMapped::AddAllNodesToLinearListAfterBuild(){
    for(auto &pair : intersectionsMapped){
        TArray<RoadIntersection> &array = pair.second;
        for (int i = 0; i < array.Num(); i++){
            RoadIntersection &current = array[i];
            if(!AllIntersections.Contains(&current)){
                AllIntersections.Add(&current);
            }
        }
    }
}






TArray<std::pair<FVector2D, FVector2D>> RoadIntersectionMapped::GetEdges(){
    TArray<std::pair<FVector2D, FVector2D>> outEdges;

    //nodes / intersections will be marked as traversed, call on every node.
    for(auto &pair : intersectionsMapped){
        TArray<RoadIntersection> &array = pair.second;
        for (int i = 0; i < array.Num(); i++){
            RoadIntersection &intersectionCurrent = array[i];
            intersectionCurrent.AppendEdges(outEdges);
        }
    }
    return outEdges;
}






//traversal build polygons
TArray<FRoadSectionList> &RoadIntersectionMapped::DisassembleTraverseGraph(){

    
    for (int i = 0; i < AllIntersections.Num(); i++)
    {
        RoadIntersection *current = AllIntersections[i];
        if(current && current->HasNeighbors()){
            DisassembleTraverseGraphFrom(current);
        }
    }
    return dissassembledSections;
}

void RoadIntersectionMapped::DisassembleTraverseGraphFrom(RoadIntersection *start){
    int maxIterations = AllIntersections.Num();
    int i = 0;

    TArray<RoadIntersection *> circle = {start};

    RoadIntersection *prev = nullptr;
    RoadIntersection *current = start;
    while(true){
        if(current == nullptr){
            return;
        }
    
        RoadIntersection *next = current->TraverseRightAndDisassembleEdge(prev, circle);
        if(next){
            /*prev = current;
            circle.Add(next);
            current = next;*/
            if(next == start){ //loop reached
                //return
                FRoadSectionList builded = BuildCirlce(circle);
                dissassembledSections.Add(builded);
                return;
            }
            prev = current;
            circle.Add(next);
            current = next;
        }else{

            // --- TODO ---
            //has no neighbors: go left from starting node until no neighbor reached?
            //or find next no neighbor / single neighbor node to close virual circle?




            FRoadSectionList builded = BuildCirlce(circle);
            dissassembledSections.Add(builded);
            return;
        }

        if(i >= maxIterations){
            return;
        }

        i++;
    }
}


#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/PolygonConstruction/RoadSection.h"
FRoadSectionList RoadIntersectionMapped::BuildCirlce(TArray<RoadIntersection *> &intersections){
    FRoadSectionList roadSections;
    for (int i = 1; i < intersections.Num(); i++)
    {
        RoadIntersection *prev = intersections[i-1];
        RoadIntersection *current = intersections[i];
        if(prev && current){
            int sharedId = -1;
            if(prev->IsAdjacentSharedRoad(*current, sharedId)){
                //connected index goes first, whether larger or smaller
                int indexA = prev->IndexForRoadId(sharedId);
                int indexB = current->IndexForRoadId(sharedId);
                
                FRoadSection section(sharedId, indexA, indexB);
                roadSections.Add(section);
            }
        }
    }
    return roadSections;
}