#include "IntersectionTraversalResult.h"

#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/Intersection.h"


IntersectionTraversalResult::IntersectionTraversalResult(){

}

IntersectionTraversalResult::~IntersectionTraversalResult(){
    intersectionPtr = nullptr;
    edgePtr = nullptr;
}

IntersectionTraversalResult::IntersectionTraversalResult(
    const IntersectionTraversalResult &other
){
    if(this != &other){
        *this = other;
    }
}

IntersectionTraversalResult &IntersectionTraversalResult::operator=(
    const IntersectionTraversalResult &other
){  
    if(this != &other){
        edgeIndex = other.edgeIndex;
        edgePtr = other.edgePtr;
        intersectionPtr = other.intersectionPtr;
    }
    return *this;
}

void IntersectionTraversalResult::SetupResult(
    int edgeIndexIn, 
    Edge *edgePtrIn, 
    Intersection *resultIntersection
){
    edgeIndex = edgeIndexIn;
    edgePtr = edgePtrIn;
    intersectionPtr = resultIntersection;
}


Intersection *IntersectionTraversalResult::GetIntersection(){
    return intersectionPtr;
}

Edge *IntersectionTraversalResult::GetEdge(){
    return edgePtr;
}

int IntersectionTraversalResult::GetEdgeIndex(){
    return edgeIndex;
}


bool IntersectionTraversalResult::ResultIsValid(){
    return edgePtr != nullptr && intersectionPtr != nullptr; //index ignored here.
}