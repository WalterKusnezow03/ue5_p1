#include "MeshedSurfaceGrid.h"
#include "GameCore/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"

FMeshedSurfaceGrid::FMeshedSurfaceGrid(){

}

FMeshedSurfaceGrid::~FMeshedSurfaceGrid(){

}

bool FMeshedSurfaceGrid::GridValid(){
    if(flagGrid.Num() == positionGrid.Num()){
        if(flagGrid.Num() > 0){
            return flagGrid[0].Num() == positionGrid[0].Num();
        }
    }
    return false;
}

void FMeshedSurfaceGrid::Init(TArray<FVector> &polygonRasterized, float widthOfInsideStep){
    stepSizeSaved = widthOfInsideStep;
    FindBounds(polygonRasterized);
    GenerateGrid();
    FlagTrue(polygonRasterized);
    FlagBetweenSpaceTrue();
}

void FMeshedSurfaceGrid::FindBounds(
    TArray<FVector> &polygon
){
    BoundingBoxSimple box(polygon);

    minSaved = box.bottomLeftNearVertex();
    maxSaved = box.topRightFarVertex();

    //debug size grid (looks ok, bool flags bricked)
    int x, y = 0;
    GetSizeGrid(x, y);
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedSurfaceGrid::sizeGrid (%d, %d)"), x, y
        )
    );
}

void FMeshedSurfaceGrid::GenerateGrid(){
    int x = -1;
    int y = -1;
    GetSizeGrid(x, y);
    if(x > 0 && y > 0){
        flagGrid.SetNum(x);
        positionGrid.SetNum(x);
        for (int i = 0; i < x; i++){
            TArray<bool> &currentFlagBuffer = flagGrid[i];
            TArray<FVector> &currentPositionBuffer = positionGrid[i];
            currentFlagBuffer.SetNum(y);
            currentPositionBuffer.SetNum(y);

            for (int j = 0; j < y; j++){

                FVector posGenerated = minSaved + FVector(
                    i * stepSizeSaved,
                    j * stepSizeSaved,
                    0
                );

                currentFlagBuffer[j] = false;
                currentPositionBuffer[j] = posGenerated;
            }
        }
    }
}

void FMeshedSurfaceGrid::GetSizeGrid(int &x, int &y){
    GetSizeGrid(x, y, stepSizeSaved);
}

void FMeshedSurfaceGrid::GetSizeGrid(int &x, int &y, float widthOfInsideStep){
    x = (maxSaved.X - minSaved.X) / widthOfInsideStep;
    y = (maxSaved.Y - minSaved.Y) / widthOfInsideStep;
}




void FMeshedSurfaceGrid::FlagTrue(const TArray<FVector> &polygon){
    for (int i = 0; i < polygon.Num(); i++){
        //deprecated, since rasterized spline might not be fully 
        //connected in step size but larger
        //FlagTrue(polygon[i]);

        int nextIndex = (i + 1) % polygon.Num();
        FlagTrueInterpolate(polygon[i], polygon[nextIndex]);
    }
}

//in between apply.
void FMeshedSurfaceGrid::FlagTrueInterpolate(
    const FVector &v0, 
    const FVector &v1
){
    FlagTrue(v0);
    FlagTrue(v1);
    float step = stepSizeSaved / 2.0f;
    float dist = FVector::Dist(v0, v1);
    int steps = dist / step;

    FVector dirStepSized = (v1 - v0).GetSafeNormal();
    dirStepSized *= step;

    for (int i = 0; i < steps + 1; i++)
    {
        FVector result = v0 + i * dirStepSized;
        FlagTrue(result);
    }
}

void FMeshedSurfaceGrid::FlagTrue(const FVector &pos){
    int x, y = 0;
    ToIndexBounded(pos, x, y);

    //if (ToIndex(pos, x ,y)){
    flagGrid[x][y] = true;

    // ------ DEBUG ------
    UWorld *world = AworldLevelBase::GetWorldPointer();
    if(world){
        FVector posCopy = positionGrid[x][y];
        DebugHelper::showLineBetween(
            world,
            posCopy,
            posCopy + FVector(0,0,10000),
            FColor::Purple,
            1000.0f
        );
        DebugHelper::showLineBetween(
            world,
            posCopy,
            pos + FVector(0,0,10000),
            FColor::Green,
            1000.0f
        );
    }
    //}
}

//once two bool flags are found, the space inbetween is marked true
void FMeshedSurfaceGrid::FlagBetweenSpaceTrue(){
    for (int i = 0; i < flagGrid.Num(); i++){
        TArray<bool> &flagBuffer = flagGrid[i];
        FlagBetweenSpaceTrue(flagBuffer);
    }
}


void FMeshedSurfaceGrid::FlagBetweenSpaceTrue(TArray<bool> &flagBuffer){
    int start = -1;
    int end = -1;
    bool startFound = false;
    bool endFound = false;

    for (int i = 0; i < flagBuffer.Num(); i++)
    {
        bool current = flagBuffer[i];
        if(!startFound && current){
            startFound = current;
            start = i;
            // reset copy
            current = false;
        }
        if(startFound && !endFound && current){
            end = i;
            //copy
            FlagBetweenSpaceTrue(flagBuffer, start, end);

            start = -1;
            end = -1;
            startFound = false;
            endFound = false;
        }
    }
}

void FMeshedSurfaceGrid::FlagBetweenSpaceTrue(TArray<bool> &flagBuffer, int i, int j){
    i = std::max(i, 0);
    j = std::min(j, flagBuffer.Num());
    for (int k = i; k < j; k++){
        flagBuffer[k] = true;
    }
}

bool FMeshedSurfaceGrid::ToIndex(const FVector &pos, int &x, int &y){
    FVector relative = pos - minSaved; //AB = B - A
    x = FMath::RoundToInt(relative.X / stepSizeSaved);
    y = FMath::RoundToInt(relative.Y / stepSizeSaved);
    
    if(x >= 0 && x < flagGrid.Num()){
        return y >= 0 && y < flagGrid[0].Num();
    }
    return false;
}

void FMeshedSurfaceGrid::ToIndexBounded(const FVector &pos, int &x, int &y){
    FVector relative = pos - minSaved; //AB = B - A
    x = FMath::RoundToInt(relative.X / stepSizeSaved);
    y = FMath::RoundToInt(relative.Y / stepSizeSaved);

    x = std::max(0, x);
    y = std::max(0, y);
    
    x = std::min(x, flagGrid.Num() - 1);
    if(x >= 0 && x < flagGrid.Num()){
        y = std::min(y, flagGrid[x].Num() - 1);
    }
}





bool FMeshedSurfaceGrid::FlagAt(int x, int y){
    if(x >= 0 && x < flagGrid.Num()){
        if (y >= 0 &&y < flagGrid[x].Num()){
            return flagGrid[x][y];
        }
    }
    return false;
}

bool FMeshedSurfaceGrid::PositionAtFlag(int x, int y, FVector &outPos){
    if(FlagAt(x,y)){
        if(x >= 0 && x < positionGrid.Num()){
            if (y >= 0 &&y < positionGrid[x].Num()){
                outPos = positionGrid[x][y];
                return true;
            }
        }
    }
    return false;
}

void FMeshedSurfaceGrid::UpdateHeights(terrainCreator *creator){
    if(!creator){
        return;
    }
    if(!GridValid()){
        return;
    }
    float offset = 30.0f;
    for (int i = 0; i < flagGrid.Num(); i++)
    {
        TArray<bool> &buffer = flagGrid[i];
        for (int j = 0; j < buffer.Num(); j++){
            if(buffer[j]){
                UpdateHeight(creator, positionGrid[i][j], offset);
            }
        }
    }
}

void FMeshedSurfaceGrid::UpdateHeight(terrainCreator *creator, FVector &vertex, float offset){
    if(creator){
        float zUpdate = creator->getHeightFor(vertex);
        vertex.Z = zUpdate + offset;
    }
}





// ------ DEBUG APPEND MESH DATA -------



void FMeshedSurfaceGrid::AppendMeshedSurface(MeshData &data){
    if(!GridValid()){
        return;
    }
    for (int i = 0; i < flagGrid.Num() - 1; i++){
        for(int j = 0; j < flagGrid[i].Num(); j++){
            AppendAt(i, j, data);
        }
    }
}

void FMeshedSurfaceGrid::AppendAt(int i, int j, MeshData &data){
    TArray<FVector> buffer = GetQuadOrTriangleAt(i, j);
    if(buffer.Num() == 3){
        data.appendEfficent(buffer[0], buffer[1], buffer[2]);
    }
    if(buffer.Num() == 4){
        data.appendEfficent(buffer[0], buffer[1], buffer[2], buffer[3]);
    }
}

TArray<FVector> FMeshedSurfaceGrid::GetQuadOrTriangleAt(int i, int j){
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
