#include "RoadSection.h"


FRoadSection::FRoadSection(){

}

FRoadSection::FRoadSection(int roadIdIn, int startIndexIn, int endIndexIn){
    Setup(roadIdIn, startIndexIn, endIndexIn);
}

FRoadSection::~FRoadSection(){

}

FRoadSection::FRoadSection(const FRoadSection &other){
    if(this != &other){
        *this = other;
    }
}
FRoadSection &FRoadSection::operator=(const FRoadSection &other){
    if(this != &other){
        roadId = other.roadId;
        startIndex = other.startIndex;
        endIndex = other.endIndex;
    }
    return *this;
}

void FRoadSection::Setup(int roadIdIn, int startIndexIn, int endIndexIn){
    roadId = roadIdIn;
    startIndex = startIndexIn;
    endIndex = endIndexIn;
}