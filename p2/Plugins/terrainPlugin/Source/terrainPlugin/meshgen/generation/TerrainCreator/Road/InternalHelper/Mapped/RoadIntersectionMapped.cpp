#include "RoadIntersectionMapped.h"


RoadIntersectionMapped::RoadIntersectionMapped(){
    //bGraphLockedState = false;
}
RoadIntersectionMapped::~RoadIntersectionMapped(){

}

bool RoadIntersectionMapped::GraphIsLocked(){
    return bGraphLockedState;
}
void RoadIntersectionMapped::LockGraph(){
    bGraphLockedState = true;
}

void RoadIntersectionMapped::Add(RoadIntersection &intersection){
    AddAsA(intersection);
    AddAsB(intersection);
}

void RoadIntersectionMapped::AddAsA(RoadIntersection &intersection){
    Add(true, intersection);
}

void RoadIntersectionMapped::AddAsB(RoadIntersection &intersection){
    Add(false, intersection);
}

void RoadIntersectionMapped::Add(bool isA, RoadIntersection &intersection){
    if(GraphIsLocked()){
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
            RoadIntersection &intersectionNext = intersectionsTracked[i];
            int index = intersectionNext.IndexForRoadId(id);
            if(index != -1){
                if(indexOfIntersection <= index){
                    intersectionsTracked.Insert(intersection, index); //right infront added, at the index as expected
                }
            }
        }
    }
}


void RoadIntersectionMapped::BuildGraph(){
    LockGraph(); //doesnt allow adding new nodes: Array might resize, all ptrs 
    //stores as adjacent neighbors will be invalid.

    BuildAdjacencyPerRoad(); //ordered linerally added one by one linked list like
    BuildAdjacencyCrossed(); //cross all roads which each other
}

void RoadIntersectionMapped::BuildAdjacencyPerRoad(){
    //build Adjacency relaations O(n^2)
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
    //build Adjacency relations O(n^2)
    for(auto &pair_i : intersectionsMapped){
        for(auto &pair_j : intersectionsMapped){
            
        }
    }
}
