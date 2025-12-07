#include "PotentialMatch.h"

PotentialMatch::PotentialMatch(){

}

PotentialMatch::~PotentialMatch(){

}

PotentialMatch::PotentialMatch(int iIn, int jIn, float distanceIn){
    Setup(iIn, jIn, distanceIn);
}

PotentialMatch::PotentialMatch(
    int iIn,
    int jIn,
    float distanceIn,
    FVector2D &postionA,
    FVector2D &postionB
){
    Setup(iIn, jIn, distanceIn, postionA, postionB);
}


PotentialMatch::PotentialMatch(const PotentialMatch &other){
    if(this != &other){
        *this = other;
    }
}
PotentialMatch &PotentialMatch::operator=(const PotentialMatch &other){
    if(this != &other){
        i = other.i;
        j = other.j;
        distance = other.distance;
        positionI = other.positionI;
        positionJ = other.positionJ;
    }
    return *this;
}

float PotentialMatch::Distance() const {
    return distance;
}

void PotentialMatch::Setup(int iIn, int jIn, float distanceIn){
    i = iIn;
    j = jIn;
    distance = distanceIn;
}

void PotentialMatch::IndexPair(int &iOut, int &jOut){
    iOut = i;
    jOut = j;
}


void PotentialMatch::Setup(
    int iIn, 
    int jIn, 
    float distanceIn, 
    FVector2D &positionA, 
    FVector2D &positionB
){
    Setup(iIn, jIn, distanceIn);
    positionI = positionA;
    positionJ = positionB;
}


FVector2D &PotentialMatch::GetPositionFirst(){
    return positionI;
}

FVector2D &PotentialMatch::GetPositionSecond(){
    return positionJ;
}