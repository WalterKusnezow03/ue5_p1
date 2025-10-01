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

int RoadIntersection::IndexForRoadId(int someRoadId){
    if(someRoadId == roadIdA){
        return indexPairOfIntersection.first;
    }
    if(someRoadId == roadIdB){
        return indexPairOfIntersection.second;
    }
    return -1;
}



void RoadIntersection::AddAsNeighborIfIsAdjacent(RoadIntersection &other){
    if(HasNeighbor(other)){
        return;
    }

    //if same road ids can be found
    TArray<int> idsThis = {RoadIdA(), RoadIdB()};
    TArray<int> idsOther = {other.RoadIdA(), other.RoadIdB()};
    for (int i = 0; i < idsThis.Num(); i++){
        int idCurrent = idsThis[i];
        for (int j = 0; j < idsOther.Num(); j++){
            if(idCurrent == idsOther[j]){

                //check if closer neighbor might exist
                //if other intersection...

                //needed here ?
                //intersections are ordered linerally anyway,
                //they can be build first,
                //then cross connections


                AddNeighbor(other);
                return;
            }
        }
    }
}

bool RoadIntersection::HasNeighbor(RoadIntersection &other){
    return adjacentNeighbors.Contains(&other);
}

void RoadIntersection::AddNeighbor(RoadIntersection &other){
    if(HasNeighbor(other)){
        return;
    }
    RoadIntersection *ptr = &other;
    adjacentNeighbors.Add(ptr);
}