#include "TwoRoadIntersection.h"


bool TwoRoadIntersection::operator==(const TwoRoadIntersection &other){
    return 
    roadIdA == other.roadIdA &&
    roadIdB == other.roadIdB &&
    indexPairOfIntersection == other.indexPairOfIntersection &&
    location == other.location;
}

void TwoRoadIntersection::Setup(
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




int TwoRoadIntersection::RoadIdA(){
    return roadIdA;
}

int TwoRoadIntersection::RoadIdB(){
    return roadIdB;
}

int TwoRoadIntersection::indexA(){
    return IndexForRoadId(RoadIdA());
}

int TwoRoadIntersection::indexB(){
    return IndexForRoadId(RoadIdB());
}

int TwoRoadIntersection::IndexForRoadId(int someRoadId){
    if(someRoadId == -1){
        return -1;
    }
    if(someRoadId == roadIdA){
        return indexPairOfIntersection.first;
    }
    if(someRoadId == roadIdB){
        return indexPairOfIntersection.second;
    }
    return -1;
}




bool TwoRoadIntersection::SharesSameRoadAndIndex(TwoRoadIntersection &other){
    int roadId = -1;
    if (IsAdjacentSharedRoad(other, roadId)){
        return IndexForRoadId(roadId) == other.IndexForRoadId(roadId);
    }
    return false;
}


bool TwoRoadIntersection::IsAdjacentSharedRoad(TwoRoadIntersection &other, int &outId){
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


bool TwoRoadIntersection::IsSame(TwoRoadIntersection &other){
    TArray<int> idsThis = {RoadIdA(), RoadIdB()};
    TArray<int> idsOther = {other.RoadIdA(), other.RoadIdB()};

    TArray<std::pair<int, int>> sharedPairs;

    for (int i = 0; i < idsThis.Num(); i++){
        int idCurrent = idsThis[i];
        for (int j = 0; j < idsOther.Num(); j++){
            int idOther = idsOther[j];
            if (idCurrent == idOther)
            {
                sharedPairs.Add(std::pair<int, int>(idCurrent, idOther));
            }
        }
    }
    if(sharedPairs.Num() == 2){
        std::pair<int, int> &firstPair = sharedPairs[0];
        std::pair<int, int> &secondPair = sharedPairs[1];


        int indexFirst_A = IndexForRoadId(firstPair.first);
        int indexFirst_B = IndexForRoadId(firstPair.second);

        int indexSecond_A = IndexForRoadId(secondPair.first);
        int indexSecond_B = IndexForRoadId(secondPair.second);





        return 
        indexFirst_A == indexFirst_B &&
        indexSecond_A == indexSecond_B &&
        indexFirst_A != -1 &&
        indexFirst_B != -1 &&
        indexSecond_A != -1 &&
        indexSecond_B != -1;

        
        //return 
        //IndexForRoadId(firstPair.first) == other.IndexForRoadId(firstPair.second) &&
        //IndexForRoadId(secondPair.first) == other.IndexForRoadId(secondPair.second);

    }
    return false;
}