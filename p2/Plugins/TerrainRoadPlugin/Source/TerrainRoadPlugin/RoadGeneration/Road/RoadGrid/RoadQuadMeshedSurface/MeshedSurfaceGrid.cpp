#include "MeshedSurfaceGrid.h"
#include "GameCore/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "terrainPluginBase/BaseTerrainInterface/TerrainInterfaceBase.h"

FMeshedSurfaceGrid::FMeshedSurfaceGrid(){

}

FMeshedSurfaceGrid::~FMeshedSurfaceGrid(){

}

FMeshedSurfaceGrid::FMeshedSurfaceGrid(const FMeshedSurfaceGrid &other){
    if(this != &other){
        *this = other;
    }
}
FMeshedSurfaceGrid &FMeshedSurfaceGrid::operator=(const FMeshedSurfaceGrid &other){
    if(this != &other){
        flagGrid = other.flagGrid;
        positionGrid = other.positionGrid;
        edgeIndices = other.edgeIndices;

        minSaved = other.minSaved;
        maxSaved = other.maxSaved;
        stepSizeSaved = other.stepSizeSaved;
        
    }   
    return *this;
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
    FlagTruePolygonEdge(v0);
    FlagTruePolygonEdge(v1);

    FVector dirStepSized;
    int steps = 0;
    GenerateStepDirectionForInterpolation(v0, v1, steps, dirStepSized);
    for (int i = 0; i < steps + 1; i++){
        FVector result = v0 + i * dirStepSized;
        FlagTruePolygonEdge(result);

        
    }
}

//generated the steps and step size for a linear interpolation across grid size to
//hit all needed poisitons
void FMeshedSurfaceGrid::GenerateStepDirectionForInterpolation(
    const FVector &v0,
    const FVector &v1,
    int &outSteps,
    FVector &outDirStepSized
){
    float step = stepSizeSaved / 2.0f;
    float dist = FVector::Dist(v0, v1);
    int steps = dist / step;

    FVector dirStepSized = (v1 - v0).GetSafeNormal();
    dirStepSized *= step;

    //copy
    outSteps = steps;
    outDirStepSized = dirStepSized;
}

//reserved for polygon method only!
void FMeshedSurfaceGrid::FlagTrue(const FVector &pos){
    int x, y = 0;
    ToIndexBounded(pos, x, y);
    flagGrid[x][y] = true;

    // ------ DEBUG ------
    /*UWorld *world = AworldLevelBase::GetWorldPointer();
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
    }*/
    
}

void FMeshedSurfaceGrid::FlagTruePolygonEdge(const FVector &pos){
    int x, y = 0;
    ToIndexBounded(pos, x, y);
    flagGrid[x][y] = true;

    std::pair<int, int> posCreated(x, y);
    if(!edgeIndices.Contains(posCreated)){
        edgeIndices.Add(posCreated);
    }
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

//bounded is needed since true flag is sometimes bricked otherwise
//because of floating point prescision errors, leading to polygon edges being OOB
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
        if (y >= 0 && y < flagGrid[x].Num()){
            return flagGrid[x][y];
        }
    }
    return false;
}

bool FMeshedSurfaceGrid::FlagAtPosition(const FVector &pos){
    int x = 0;
    int y = 0;
    ToIndexBounded(pos, x, y);
    return FlagAt(x, y);
}

bool FMeshedSurfaceGrid::PositionAtFlag(int x, int y, FVector &outPos){
    if(FlagAt(x,y)){
        return PositionAt(x, y, outPos);
    }
    return false;
}

bool FMeshedSurfaceGrid::PositionAt(const std::pair<int,int> &pair, FVector &outPos){
    return PositionAt(pair.first, pair.second, outPos);
}

bool FMeshedSurfaceGrid::PositionAt(int x, int y, FVector &outPos){
    if(x >= 0 && x < positionGrid.Num()){
        if (y >= 0 &&y < positionGrid[x].Num()){
            outPos = positionGrid[x][y];
            return true;
        }
    }
    return false;
}

FVector FMeshedSurfaceGrid::GetPositionAt(const std::pair<int,int> &pair){
    //copy
    FVector returned(0, 0, 0);
    PositionAt(pair, returned);
    return returned;
}

FVector FMeshedSurfaceGrid::Rotation(
    const std::pair<int, int> &posAPair,
    const std::pair<int, int> &posBPair
){
    // --- TODO ---
    FVector posAVec = GetPositionAt(posAPair);
    FVector posBVec = GetPositionAt(posBPair);
    posAVec.Z = 0.0f;
    posBVec.Z = 0.0f;
    FVector dir = posBVec - posAVec; // AB = B - A
    return dir.GetSafeNormal();
}

void FMeshedSurfaceGrid::UpdateHeights(TerrainInterfaceBase *creator){
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

void FMeshedSurfaceGrid::UpdateHeight(TerrainInterfaceBase *creator, FVector &vertex, float offset){
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




/// --- finding shapes to fit ---


/*
We need to iterate over the edge and find some
which matches the desired width x
Once found, the rotated area must be checked for all flags false
updated and then return whether the result was sucessfull
*/

// is NOT tested!
bool FMeshedSurfaceGrid::FindShape(
    int x, //in cm
    int y, //in cm
    FVector &outBottomLeft,
    FVector &outRotation
){
    //align rotation included, y is always depth, x side

    for (int i = 0; i < edgeIndices.Num(); i++){
        int j = (i + 1) % edgeIndices.Num();

        const std::pair<int, int> &posA = edgeIndices[i];
        const std::pair<int, int> &posB = edgeIndices[j];
        if(DistanceEnough(edgeIndices[i], edgeIndices[j], x)){
            
            FVector bottomLeft = GetPositionAt(posA);
            FVector rotation = Rotation(posA, posB);
            if(LockArea(posA, posB, rotation, y)){ //y depth along edge to inside
                return true;
            }
        }
    }
    return false;
}

bool FMeshedSurfaceGrid::DistanceEnough(
    const std::pair<int, int> &posA,
    const std::pair<int, int> &posB,
    int desired
){
    return Distance(posA, posB) <= desired;
}

int FMeshedSurfaceGrid::Distance(
    const std::pair<int, int> &posA,
    const std::pair<int, int> &posB
){
    FVector convertedA(posA.first, posA.second, 0.0f);
    FVector convertedB(posB.first, posB.second, 0.0f);
    return FVector::Dist(convertedA, convertedB);
}




// lock area 
bool FMeshedSurfaceGrid::LockArea(
    const std::pair<int, int> &posA,
    const std::pair<int, int> &posB,
    FVector &rotation,
    int depth //in cm
){
    /*
    prüfen ob die vertecies an den interpolierten 
    stellen frei sind
    */
    FVector orthogonal(rotation.Y, -rotation.X, 0.0f);
    orthogonal = orthogonal.GetSafeNormal() * depth;

    FVector aPosVector = GetPositionAt(posA);
    FVector bPosVector = GetPositionAt(posB);

    FVector extendedA = aPosVector + orthogonal;
    FVector extendedB = bPosVector + orthogonal;


    // ---- MOVE TO FVECTOR UTIL ? ----
    FVector dirStepAB;
    int stepsAB = 0;
    GenerateStepDirectionForInterpolation(aPosVector, bPosVector, stepsAB, dirStepAB);

    FVector dirStepAextendA;
    int stepsAextendA = 0;
    GenerateStepDirectionForInterpolation(aPosVector, bPosVector, stepsAextendA, dirStepAextendA);


    //bilinear interpolieren und prüfen! 
    TArray<std::pair<int, int>> markedForLock;
    for (int i = 0; i < stepsAB + 1; i++)
    {
        //step along ab
        FVector stepABWorld = aPosVector + dirStepAB * i;
        for (int j = 0; j < stepsAextendA + 1; j++){
            FVector stepLocal = dirStepAextendA * j;
            FVector bilinearInterpolate = stepABWorld + stepLocal;

            //generate index
            int x = 0;
            int y = 0;
            ToIndexBounded(bilinearInterpolate, x, y);
            if(FlagAt(x,y)){
                //mark for remove
                markedForLock.Add(std::pair<int, int>(x, y));
            }else{
                //at least one is not enough
                return false;
            }
        }
    }
    SetFlag(markedForLock, false);
    return true;
}

void FMeshedSurfaceGrid::SetFlag(
    const TArray<std::pair<int, int>> &indexPositions, 
    bool flag
){
    for (int i = 0; i < indexPositions.Num(); i++){
        const std::pair<int, int> &current = indexPositions[i];
        SetFlag(current, flag);
    }
}

void FMeshedSurfaceGrid::SetFlag(
    const std::pair<int, int> &indexPos, 
    bool flag
){
    int x = indexPos.first;
    int y = indexPos.second;
    if (x >= 0 && x < flagGrid.Num())
    {
        if (y >= 0 && y < flagGrid[x].Num()){
            flagGrid[x][y] = flag;
        }
    }
}

