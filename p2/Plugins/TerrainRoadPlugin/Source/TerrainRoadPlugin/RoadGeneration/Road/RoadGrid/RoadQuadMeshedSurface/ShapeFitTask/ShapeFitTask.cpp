#include "ShapeFitTask.h"


void ShapeFitTask::Setup(int x, int y){
    sizeX = std::abs(x);
    sizeY = std::abs(y);
}

void ShapeFitTask::SetPivot(const FVector &pivotIn){
    pivot = pivotIn;
}

void ShapeFitTask::UpdatePivotPickLowestZ(const TArray<FVector> &array){
    for (int i = 0; i < array.Num(); i++){
        const FVector &current = array[i];
        pivot.Z = std::min(current.Z, pivot.Z);
    }
}

void ShapeFitTask::GetSize(int &outX, int &outY){
    outX = sizeX;
    outY = sizeY;
}


void ShapeFitTask::UpdateRotation(const FRotator &in){
    rotator = in;
}

const FVector &ShapeFitTask::GetPivot() const {
    return pivot;
}

const FRotator &ShapeFitTask::GetRotation() const{
    return rotator;
}

FRotator &ShapeFitTask::GetRotationRef(){
    return rotator;
}