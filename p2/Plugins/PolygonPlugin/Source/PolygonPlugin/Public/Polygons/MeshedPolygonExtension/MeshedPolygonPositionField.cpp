#include "MeshedPolygonPositionField.h"






bool FMeshedPolygonPositionField::GridValid() const{
    if(flagGrid.Num() == positionGrid.Num()){
        if(flagGrid.Num() > 0){
            return flagGrid[0].Num() == positionGrid[0].Num();
        }
    }
    return false;
}


void FMeshedPolygonPositionField::UpdateWorldHeightFor(FVector &asWorldPos){
    std::pair<int, int> pivot;
    ToIndexBounded(asWorldPos, pivot);

    //get world pos from grid
    FVector worldPosZ = GetPositionAt(pivot);
    asWorldPos.Z = worldPosZ.Z;
}



bool FMeshedPolygonPositionField::InitAsSinglePixel(TArray<FVector> &polygon, float widthOfInsideStep){
    if(polygon.Num() <= 0){
        return true;
    }

    BoundingBoxSimple box(polygon);
    float size = FVector::Dist(box.bottomLeftNearVertex(), box.topRightFarVertex());
    //force one pixel size
    if(size <= widthOfInsideStep){
        stepSizeSaved = widthOfInsideStep;

        flagGrid.SetNum(1);
        flagGrid[0].SetNum(1);
        flagGrid[0][0] = FlagAsInt8(true); //(uint8)1; // true;

        positionGrid.SetNum(1);
        positionGrid[0].SetNum(1);
        positionGrid[0][0] = box.bottomLeftNearVertex();

        minSaved = box.bottomLeftNearVertex();
        maxSaved = box.bottomLeftNearVertex() + FVector(0, widthOfInsideStep, widthOfInsideStep);

        DebugHelper::logMessage("FMeshedPolygon::Inited As Single Pixel");

        return true;
    }
    return false;
}


const TArray<TArray<FVector>> &FMeshedPolygonPositionField::GetPositionGrid() const {
    return positionGrid;
}

TArray<TArray<FVector>> &FMeshedPolygonPositionField::GetPositionGrid(){
    return positionGrid;
}


FVector FMeshedPolygonPositionField::BottomLeft(){
    if(GridValid()){
        return positionGrid[0][0];
    }
    return FMeshedPolygon::BottomLeft();
}

FVector FMeshedPolygonPositionField::TopRight(){
    if(GridValid()){
        TArray<FVector> &maxCol = positionGrid.Last();
        FVector &last = maxCol.Last();
        return last;
    }
    return FMeshedPolygon::TopRight();
}

void FMeshedPolygonPositionField::GenerateGrid(){
    int x = -1;
    int y = -1;
    GetSizeGrid(x, y);
    if(x > 0 && y > 0){
        DebugHelper::logMessage(FString::Printf(TEXT("FMeshedPolygon::GenerateGrid x:%d y:%d"), x, y));
        if (
            TGridIsSize<uint8>(x, y, flagGrid) &&
            TGridIsSize<FVector>(x, y, positionGrid))
        {
            ClearFlags();
            return;
        }

        TGenerateGrid<uint8>(x, y, flagGrid);
        TGenerateGrid<FVector>(x, y, positionGrid);
        ClearFlags();
        MakePositionGrid();
    }
}


void FMeshedPolygonPositionField::MakePositionGrid(){
    for (int i = 0; i < positionGrid.Num(); i++){
        TArray<FVector> &currentPositionBuffer = positionGrid[i];
        for (int j = 0; j < currentPositionBuffer.Num(); j++){
            FVector posGenerated = minSaved + FVector(
                i * stepSizeSaved,
                j * stepSizeSaved,
                0
            );
            currentPositionBuffer[j] = posGenerated;
        }
    }
}


bool FMeshedPolygonPositionField::PositionAtFlag(int x, int y, FVector &outPos){
    if(FlagAt(x,y)){
        return PositionAt(x, y, outPos);
    }
    return false;
}

bool FMeshedPolygonPositionField::PositionAt(const std::pair<int,int> &pair, FVector &outPos){
    return PositionAt(pair.first, pair.second, outPos);
}

bool FMeshedPolygonPositionField::PositionAt(int x, int y, FVector &outPos){
    if(x >= 0 && x < positionGrid.Num()){
        if (y >= 0 &&y < positionGrid[x].Num()){
            outPos = positionGrid[x][y];
            return true;
        }
    }
    return false;
}

FVector FMeshedPolygonPositionField::GetPositionAt(const std::pair<int,int> &pair){
    //copy
    FVector returned(0, 0, 0);
    PositionAt(pair, returned);
    return returned;
}




// ------ DEBUG APPEND MESH DATA -------

void FMeshedPolygonPositionField::AppendMeshedSurface(MeshData &data){
    AppendMeshedSurface(data, false);
}

//true for local mesh
void FMeshedPolygonPositionField::AppendMeshedSurface(MeshData &data, bool removeOffset2D){
    if(!GridValid()){
        return;
    }
    for (int i = 0; i < flagGrid.Num() - 1; i++){
        for(int j = 0; j < flagGrid[i].Num(); j++){
            AppendAt(i, j, data, removeOffset2D);
        }
    }
}

//true for local mesh
void FMeshedPolygonPositionField::AppendAt(int i, int j, MeshData &data, bool removeOffset2D){
    TArray<FVector> buffer = GetQuadOrTriangleAt(i, j);

    if(removeOffset2D){
        FVector pivot = BottomLeft();
        //pivot.Z = 0.0f;
        for (int v = 0; v < buffer.Num(); v++)
        {
            buffer[v] = buffer[v] - pivot; //AB = B - A
        }
    }

    if(buffer.Num() == 3){
        data.appendEfficent(buffer[0], buffer[1], buffer[2]);
    }
    if(buffer.Num() == 4){
        data.appendEfficent(buffer[0], buffer[1], buffer[2], buffer[3]);
    }
}

TArray<FVector> FMeshedPolygonPositionField::GetQuadOrTriangleAt(int i, int j){
    TArray<FVector> outBuffer;
    //append quad
    /*
    1->2
    |  |
    0<-3
    */
    FVector pos;
    if (PositionAtFlag(i, j, pos)){
        outBuffer.Add(pos);
    }
    if (PositionAtFlag(i, j+1, pos)){
        outBuffer.Add(pos);
    }
    if (PositionAtFlag(i+1, j+1, pos)){
        outBuffer.Add(pos);
    }
    if (PositionAtFlag(i+1, j, pos)){
        outBuffer.Add(pos);
    }
    return outBuffer;
}



void FMeshedPolygonPositionField::SetPosition(int x, int y, const FVector &pos){
    if (x >= 0 && x < positionGrid.Num())
    {
        if (y >= 0 && y < positionGrid[x].Num()){
            positionGrid[x][y] = pos;
        }
    }
}

