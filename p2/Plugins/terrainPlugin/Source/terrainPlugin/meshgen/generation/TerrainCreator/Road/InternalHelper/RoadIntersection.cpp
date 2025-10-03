#include "RoadIntersection.h"

RoadIntersection::RoadIntersection(){

}

RoadIntersection::~RoadIntersection(){

}

RoadIntersection::RoadIntersection(const RoadIntersection &other){
    if(this != &other){
        *this = other;
    }
}

RoadIntersection &RoadIntersection::operator=(const RoadIntersection &other){
    if(this != &other){
        roadIdA = other.roadIdA;
        roadIdB = other.roadIdB;
        indexPairOfIntersection = other.indexPairOfIntersection;
        location = other.location;
    }
    return *this;
}

FString RoadIntersection::ToString(){
    FString msg = FString::Printf(
        TEXT("roadIdA(%d) roadIdB(%d) indexPairOfIntersection(%d,%d)"),
        roadIdA,
        roadIdB,
        indexPairOfIntersection.first,
        indexPairOfIntersection.second
    );
    return msg;
}

void RoadIntersection::Setup(
    int roadIdAIn, 
    int roadIdBIn, 
    const std::pair<int, int> &indexPair,
    FVector2D &locationIn
){
    roadIdA = roadIdAIn;
    roadIdB = roadIdBIn;
    indexPairOfIntersection = indexPair;
    location = locationIn;
}


int RoadIntersection::RoadIdA(){
    return roadIdA;
}

int RoadIntersection::RoadIdB(){
    return roadIdB;
}

int RoadIntersection::indexA(){
    return IndexForRoadId(RoadIdA());
}

int RoadIntersection::indexB(){
    return IndexForRoadId(RoadIdB());
}

int RoadIntersection::IndexForRoadId(int someRoadId){
    if(someRoadId == roadIdA){
        return indexPairOfIntersection.first;
    }
    if(someRoadId == roadIdB){
        return indexPairOfIntersection.second;
    }
    return -1;
}




/// --- experimental push in before a node and re-adjust link---
void RoadIntersection::ConnectBiDirectionalIfIsAdjacentAndRelinkNodes(RoadIntersection &other){
    int indexToRelinkPrev = -1;
    int indexToRelinkNext = -1;
    if(IsAdjacent(other)){
        RoadIntersection *laterNode = nullptr;
        if (HasLaterNode(other, laterNode))
        {
            if(laterNode){
                laterNode->RemoveNeighbor(*this);
                this->RemoveNeighbor(*laterNode);

                laterNode->AddNeighbor(other);
                this->AddNeighbor(other);
                other.AddNeighbor(*this);
            }
        }
        else
        {
            AddNeighbor(other);
            other.AddNeighbor(*this);
        }
    }
}

bool RoadIntersection::HasLaterNode(
    RoadIntersection &other,
    RoadIntersection *&outLaterNode
){
    int sharedRoadId = -1;
    if(IsAdjacentSharedRoad(other, sharedRoadId)){
        int splineIndex = other.IndexForRoadId(sharedRoadId);

        TArray<std::pair<RoadIntersection *, int>> intersections = SortNeighborsByIndexSharedThisRoad(
            other
        );

        //find pair between, relink
        for (int i = 0; i < intersections.Num(); i++){
            std::pair<RoadIntersection *, int> &current = intersections[i];
            int index = current.second;
            
            if(splineIndex <= index){
                outLaterNode = current.first;
                return true;
            }
        }
    }

    return false;
}







TArray<std::pair<RoadIntersection*, int>> RoadIntersection::SortNeighborsByIndexSharedThisRoad(
    RoadIntersection &other
){
    int sharedRoadId = -1;
    TArray<std::pair<RoadIntersection*, int>> outArray;
    if (IsAdjacentSharedRoad(other, sharedRoadId))
    {
        for (int i = 0; i < adjacentNeighbors.Num(); i++){
            if(RoadIntersection *current = adjacentNeighbors[i]){

                //copy in index of intersection
                int indexIntersectionInRoad = current->IndexForRoadId(sharedRoadId);
                std::pair<RoadIntersection *, int> newPair(current, indexIntersectionInRoad);
                outArray.Add(newPair);
            }
        }
    }

    outArray.Sort([](
        const std::pair<RoadIntersection*, int> &A, 
        const std::pair<RoadIntersection*, int> &B
    ) {
        return A.second < B.second; // aufsteigend
    });

    return outArray;
}










void RoadIntersection::ConnectBiDirectionalIfIsAdjacent(RoadIntersection &other){
    if(AddAsNeighborIfIsAdjacent(other)){
        other.AddNeighbor(*this);
    }
}


bool RoadIntersection::AddAsNeighborIfIsAdjacent(RoadIntersection &other){
    if(HasNeighbor(other)){
        return false;
    }

    if(IsAdjacent(other)){
        return AddNeighbor(other);
    }
    return false;
}

bool RoadIntersection::IsAdjacent(RoadIntersection &other){
    int ignored = -1;
    return IsAdjacentSharedRoad(other, ignored);
}

bool RoadIntersection::IsAdjacentSharedRoad(RoadIntersection &other, int &outId){
    TArray<int> idsThis = {RoadIdA(), RoadIdB()};
    TArray<int> idsOther = {other.RoadIdA(), other.RoadIdB()};
    for (int i = 0; i < idsThis.Num(); i++){
        int idCurrent = idsThis[i];
        for (int j = 0; j < idsOther.Num(); j++){
            if(idCurrent == idsOther[j]){
                outId = idCurrent;
                return true;
            }
        }
    }
    return false;
}




bool RoadIntersection::HasNeighbor(RoadIntersection &other){
    return adjacentNeighbors.Contains(&other);
}

bool RoadIntersection::AddNeighbor(RoadIntersection &other){
    if(HasNeighbor(other)){
        return false;
    }
    RoadIntersection *ptr = &other;
    adjacentNeighbors.Add(ptr);
    return true;
}


void RoadIntersection::RemoveNeighbor(RoadIntersection &other){
    int32 Index = adjacentNeighbors.Find(&other);
    if (Index != INDEX_NONE)
    {
        adjacentNeighbors[Index] = adjacentNeighbors.Last();
        adjacentNeighbors.Pop();
    }
}






//// ---- edges / traversal ----

void RoadIntersection::AppendEdges(TArray<std::pair<FVector2D, FVector2D>> &edges){
    bTraversed = true;

    std::pair<FVector2D, FVector2D> pairEdge;
    pairEdge.first = location;
    for (int i = 0; i < adjacentNeighbors.Num(); i++)
    {
        if(RoadIntersection *current = adjacentNeighbors[i]){
            if(!current->IsTraversed()){
                pairEdge.second = current->location;
                edges.Add(pairEdge);
            }
        }
    }
}


bool RoadIntersection::IsTraversed(){
    return bTraversed;
}