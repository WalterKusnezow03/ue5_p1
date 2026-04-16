#include "MeshedPolygonDistanceField.h"


void FMeshedPolygonDistanceField::Init(FMeshedPolygon &polygon){
    if(!polygon.IsValid()){
        return;
    }

    TArray<TArray<bool>> &flagGrid = polygon.GetFlagGrid();
    TArray<TArray<FVector>> &positionGrid = polygon.GetPositionGrid();
    int sizeX = flagGrid.Num();
    int sizeY = flagGrid[0].Num();
    if(sizeX > 0 && sizeY > 0){
        TGenerateGrid<float>(sizeX, sizeY, distanceField);
        InitGridMax();
        GenerateDistanceField(flagGrid, positionGrid);
    }
}

void FMeshedPolygonDistanceField::InitGridMax(){
    for (int i = 0; i < distanceField.Num(); i++){
        TArray<float> &current = distanceField[i];
        for (int j = 0; j < current.Num(); j++){
            current[j] = FLT_MAX;
        }
    }
}




void FMeshedPolygonDistanceField::GenerateDistanceField(
    TArray<TArray<bool>> &flagGrid,
    TArray<TArray<FVector>> &positionGrid
){
    //columns
    for (int i = 0; i < flagGrid.Num(); i++){
        TArray<bool> &columnBool = flagGrid[i];
        TArray<FVector> &columnPos = positionGrid[i];
        TArray<float> &distanceFieldColumn = distanceField[i];
        GenerateDistanceFieldColumnIncrement(
            columnBool,
            columnPos,
            distanceFieldColumn
        );
        GenerateDistanceFieldColumnDecrement(
            columnBool,
            columnPos,
            distanceFieldColumn
        );
    }
    //rows

}


void FMeshedPolygonDistanceField::GenerateDistanceFieldColumnIncrement(
    TArray<bool> &flagColumn,
    TArray<FVector> &positionColumn,
    TArray<float> &outColumn
){
    //if true: check distance
    // true: blocked, false: free
    int trueIndex = -1;
    for (int i = 0; i < flagColumn.Num(); i++){
        if(flagColumn[i]){
            trueIndex = i;
        }
        if(InBound<FVector>(positionColumn, i) && InBound<FVector>(positionColumn, trueIndex)){
            float dist = FVector::Dist(positionColumn[i], positionColumn[trueIndex]);
            outColumn[i] = std::min(dist, outColumn[i]);
        }
    }
}

void FMeshedPolygonDistanceField::GenerateDistanceFieldColumnDecrement(
    TArray<bool> &flagColumn,
    TArray<FVector> &positionColumn,
    TArray<float> &outColumn
){
    int trueIndex = flagColumn.Num();
    for (int i = flagColumn.Num() - 1; i >= 0; i--){
        if(flagColumn[i]){
            trueIndex = i;
        }
        if(InBound<FVector>(positionColumn, i) && InBound<FVector>(positionColumn, trueIndex)){
            float dist = FVector::Dist(positionColumn[i], positionColumn[trueIndex]);
            outColumn[i] = std::min(dist, outColumn[i]);
        }
    }
}



//make columns!








/*
richtung x und y wandern, wenn true: wall, 




TArray<TArray<bool>> flagGrid;
TArray<TArray<FVector>> positionGrid;


*/

