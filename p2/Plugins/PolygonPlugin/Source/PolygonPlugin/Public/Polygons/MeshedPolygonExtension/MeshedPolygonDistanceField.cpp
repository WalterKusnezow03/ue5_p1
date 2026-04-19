#include "MeshedPolygonDistanceField.h"



void FMeshedPolygonDistanceField::InitDistanceField(){
    if(!IsValid()){
        DebugHelper::logMessage("FMeshedPolygonDistanceField::GridNotValid");
        return;
    }

    TArray<TArray<uint8>> &flagGrid = GetFlagGrid();
    TArray<TArray<FVector>> &positionGrid = GetPositionGrid();
    int sizeX = flagGrid.Num();
    int sizeY = flagGrid[0].Num();
    if(sizeX > 0 && sizeY > 0){
        TGenerateGrid<float>(sizeX, sizeY, distanceField);
        InitGridMax();
        GenerateDistanceField(flagGrid, positionGrid);
        DebugHelper::logMessage("FMeshedPolygonDistanceField::DistanceFieldInited");
    }
}

FString FMeshedPolygonDistanceField::ToString(){
    FString outString = TEXT("FMeshedPolygonDistanceField::ToString::");
    for (int i = 0; i < distanceField.Num(); i++){
        TArray<float> &buffer = distanceField[i];
        for (int j = 0; j < buffer.Num(); j++){
            outString += FString::Printf(TEXT(" %.1f"), buffer[j]);
        }
    }
    return outString;
}

void FMeshedPolygonDistanceField::InitGridMax(){
    int x = sizeX();
    int y = sizeY();
    int max = stepSizeSaved * std::max(x, y);
    for (int i = 0; i < distanceField.Num(); i++){
        TArray<float> &current = distanceField[i];
        for (int j = 0; j < current.Num(); j++){
            current[j] = max;
        }
    }
}




void FMeshedPolygonDistanceField::GenerateDistanceField(
    TArray<TArray<uint8>> &flagGrid,
    TArray<TArray<FVector>> &positionGrid
){
    //columns
    for (int i = 0; i < flagGrid.Num(); i++){
        TArray<uint8> &columnBool = flagGrid[i];
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
    if(flagGrid.Num() > 0){
        TArray<uint8> &col = flagGrid[0];
        for (int row = 0; row < col.Num(); row++){
            GenerateDistanceFieldRowIncrement(
                flagGrid,
                positionGrid,
                distanceField,
                row
            );
            GenerateDistanceFieldRowDecrement(
                flagGrid,
                positionGrid,
                distanceField,
                row
            );
        }
    }
}


void FMeshedPolygonDistanceField::GenerateDistanceFieldColumnIncrement(
    TArray<uint8> &flagColumn,
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
    TArray<uint8> &flagColumn,
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

void FMeshedPolygonDistanceField::GenerateDistanceFieldRowIncrement(
    TArray<TArray<uint8>> &flagGrid,
    TArray<TArray<FVector>> &positionGrid,
    TArray<TArray<float>> &outGrid,
    int row
){
    //[column][column][column]...
    int trueIndex = -1;
    for (int i = 0; i < flagGrid.Num(); i++){
        TArray<uint8> &column = flagGrid[i];
        if(row >= 0 && row < column.Num()){
            bool flag = column[row] == (uint8) 1;
            if(flag){
                trueIndex = i;
            }

            if(InBound<TArray<FVector>>(positionGrid, trueIndex)){
                const FVector& p0 = positionGrid[i][row];
                const FVector& p1 = positionGrid[trueIndex][row];
                float dist = FVector::Dist(p0, p1);
                outGrid[i][row] = std::min(dist, outGrid[i][row]);
            }
        }
    }
}



void FMeshedPolygonDistanceField::GenerateDistanceFieldRowDecrement(
    TArray<TArray<uint8>> &flagGrid,
    TArray<TArray<FVector>> &positionGrid,
    TArray<TArray<float>> &outGrid,
    int row
){
    //[column][column][column]...
    int trueIndex = flagGrid.Num();
    for (int i = flagGrid.Num() - 1; i >= 0; i--){
        TArray<uint8> &column = flagGrid[i];
        if(row >= 0 && row < column.Num()){
            bool flag = column[row] == (uint8) 1;
            if(flag){
                trueIndex = i;
            }

            if(InBound<TArray<FVector>>(positionGrid, trueIndex)){
                const FVector& p0 = positionGrid[i][row];
                const FVector& p1 = positionGrid[trueIndex][row];
                float dist = FVector::Dist(p0, p1);
                outGrid[i][row] = std::min(dist, outGrid[i][row]);
            }
        }
    }
}









/*
richtung x und y wandern, wenn true: wall, 




TArray<TArray<bool>> flagGrid;
TArray<TArray<FVector>> positionGrid;


*/






FColor FMeshedPolygonDistanceField::ColorDirection(const FColor &a, const FColor &b) const{
    //AB = B - A
    FColor dir;
    dir.R = (b.R - a.R);
    dir.G = (b.G - a.G);
    dir.B = (b.B - a.B);
    return dir;
}

FColor FMeshedPolygonDistanceField::ScaledColor(
    const FColor &start, 
    const FColor &end, 
    float scalar
)const{
    float t = FMath::Clamp(scalar, 0.0f, 1.0f);

    FLinearColor la(start);
    FLinearColor lb(end);

    FLinearColor result = FMath::Lerp(la, lb, t);
    return result.ToFColor(true);
    
    
    
    
    /*FColor a;
    //gx: A + t * (B - A)
    a.R = start.R + dir.R * scalar;
    a.G = start.G + dir.G * scalar;
    a.B = start.B + dir.B * scalar;
    Validate(a);
    return a;*/
}

void FMeshedPolygonDistanceField::Validate(FColor &color)const{
    uint8 min = (uint8)0;
    uint8 max = (uint8)255;
    color.R = std::max(color.R, min);
    color.G = std::max(color.G, min);
    color.B = std::max(color.B, min);

    color.R = std::min(color.R, max);
    color.G = std::min(color.G, max);
    color.B = std::min(color.B, max);
}

float FMeshedPolygonDistanceField::MaxDistance() const{
    float outDist = 0.0f;
    for (int i = 0; i < distanceField.Num(); i++){
        outDist = std::max(outDist, MaxDistance(distanceField[i]));
    }
    return outDist;
}

float FMeshedPolygonDistanceField::MaxDistance(const TArray<float> &buffer) const{
    float max = 0.0f;
    for (int i = 0; i < buffer.Num(); i++){
        max = std::max(max, buffer[i]);
    }
    return max;
}




//color overriden method

// ---- testing needed ! ----
void FMeshedPolygonDistanceField::GenerateColorBitmap(
    TArray<FColor> &outBuffer,
    FColor &freeColor,
    FColor &blockedEnd,
    int &resXOut,
    int &resYOut
) const {
    DebugHelper::logMessage("FMeshedPolygonDistanceField::GenerateColorBitmap A");
    if(IsValidDistanceField()){
        //distance field and flag res are the same
        GetResolution(resXOut, resYOut);
        GenerateColorBitmap(outBuffer, freeColor, blockedEnd);
        DebugHelper::logMessage("FMeshedPolygonDistanceField::GenerateColorBitmap B");
    }
    DebugHelper::logMessage("FMeshedPolygonDistanceField::GenerateColorBitmap BufferOut", outBuffer.Num());
}

void FMeshedPolygonDistanceField::GenerateColorBitmap(
    TArray<FColor> &outBuffer,
    FColor &freeColor,
    FColor &blockedEnd
) const {
    float maxDistance = MaxDistance();
    DebugHelper::logMessage("FMeshedPolygonDistanceField::maxDistance ",maxDistance);
    //FColor dir = ColorDirection(freeColor, blockedEnd);

    for (int i = 0; i < distanceField.Num(); i++){
        const TArray<float> &current = distanceField[i];
        GenerateColorBitmap(
            outBuffer,
            freeColor,
            blockedEnd,
            current,
            maxDistance //for color scaling
        );
    }
    
    
}


void FMeshedPolygonDistanceField::GenerateColorBitmap(
    TArray<FColor> &outBuffer,
    FColor &freeColor,
    FColor &end,
    const TArray<float> &distanceFieldColumn,
    float maxDistance
) const {
    int prevSize = outBuffer.Num();
    outBuffer.SetNumUninitialized(prevSize + distanceFieldColumn.Num());
    for (int i = 0; i < distanceFieldColumn.Num(); i++){
        float distCurrent = distanceFieldColumn[i]; //0 ggf
        float scalar = distCurrent / maxDistance; // scalar = distTarget / distAll
        //invertScalar = 1.0 - scalar
        float invertedScalar = 1.0f - scalar;

        FColor generated = ScaledColor(freeColor, end, invertedScalar);
        outBuffer[prevSize + i] = generated;
    }
}



bool FMeshedPolygonDistanceField::IsValidDistanceField() const {
    if(distanceField.Num() > 0){
        return distanceField[0].Num() > 0;
    }
    return false;
}