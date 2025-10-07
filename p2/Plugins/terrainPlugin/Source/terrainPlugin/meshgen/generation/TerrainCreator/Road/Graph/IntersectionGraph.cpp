#include "IntersectionGraph.h"

#include "GameCore/DebugHelper.h"


IntersectionGraph::IntersectionGraph(){
    bGraphLockedState = false;
}
IntersectionGraph::~IntersectionGraph(){

}





bool IntersectionGraph::GraphIsLocked(){
    return bGraphLockedState;
}
void IntersectionGraph::LockGraph(){
    bGraphLockedState = true;
}

void IntersectionGraph::Add(TwoRoadIntersection &other){
    if(GraphIsLocked()){
        return; //array resize might happen, locked is locked and stays locked after adjacency build.
    }
    bool foundJoin = false;
    for (int i = 0; i < nodes.Num(); i++){
        Intersection &current = nodes[i];
        if(current.Join(other)){
            foundJoin = true;
            break;
        }
    }

    if (!foundJoin)
    {
        Intersection newIntersection(other);
        nodes.Add(newIntersection);
    }
}

void IntersectionGraph::BuildGraph(){
    LockGraph(); //doesnt allow adding new nodes: Array might resize, all ptrs 
    //stores as adjacent neighbors will be invalid.
    BuildAdjacencyCrossed(); //cross all roads which each other
}


void IntersectionGraph::BuildAdjacencyCrossed(){
   
    //O(n^2)
    for (int i = 0; i < nodes.Num(); i++){
        Intersection &intersectionCurrent_i = nodes[i];
        for (int j = i; j < nodes.Num(); j++)
        {
            Intersection &intersectionCurrent_j = nodes[j];
            intersectionCurrent_i.AddAsNeighborIfAdjacent(&intersectionCurrent_j);
        }
    }
}



TArray<std::pair<FVector2D, FVector2D>> IntersectionGraph::GetEdges(){
    TArray<std::pair<FVector2D, FVector2D>> outEdges;

    for (int i = 0; i < nodes.Num(); i++){
        Intersection &current = nodes[i];
        current.AppendEdges(outEdges);
    }
    return outEdges;
}







///// ----- TRAVERSAL PARTIALLY BRICKED ------

//traversal build polygons
TArray<FRoadSectionList> &IntersectionGraph::DisassembleTraverseGraph(){

    for (int i = 0; i < nodes.Num(); i++)
    {
        Intersection &current = nodes[i];
        if(current.HasNeighbors()){
            DisassembleTraverseGraphFrom(&current);
        }
    }
    return dissassembledSections;
}

void IntersectionGraph::DisassembleTraverseGraphFrom(Intersection *start){
    int maxIterations = nodes.Num();
    int i = 0;
    FRoadSectionList result;

    Intersection *prev = nullptr;
    Intersection *current = start;

    TArray<Intersection *> circle = {current};
    while(true){
        if(current == nullptr){
            return;
        }
    
        Intersection *next = current->TraverseRightAndDisassembleEdge(prev, circle, result);
        if(next){
            prev = current;
            circle.Add(next);
            current = next;
            if(next == start){ //loop reached
                //return
                dissassembledSections.Add(result);
                return;
            }
            
        }else{

            // --- TODO ---
            //has no neighbors: go left from starting node until no neighbor reached?
            //or find next no neighbor / single neighbor node to close virual circle?




            dissassembledSections.Add(result);
            return;
        }

        if(i >= maxIterations){
            return;
        }

        i++;
    }
}