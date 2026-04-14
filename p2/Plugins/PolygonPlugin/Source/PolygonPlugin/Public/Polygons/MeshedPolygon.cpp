#include "MeshedPolygon.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"
#include "DebugPlugin/DebugHelper.h"

#include "PolygonPlugin/Public/Polygons/rasterizer/CurveRasterizer.h"

FMeshedPolygon::FMeshedPolygon(){

}

FMeshedPolygon::~FMeshedPolygon(){

}

FMeshedPolygon::FMeshedPolygon(const FMeshedPolygon &other){
    if(this != &other){
        *this = other;
    }
}
FMeshedPolygon &FMeshedPolygon::operator=(const FMeshedPolygon &other){
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

bool FMeshedPolygon::IsValid(){
    return GridValid();
}

FVector FMeshedPolygon::BottomLeft(){
    if(GridValid()){
        return positionGrid[0][0];
    }
    return FVector(0, 0, 0);
}

FVector FMeshedPolygon::TopRight(){
    if(GridValid()){
        TArray<FVector> &maxCol = positionGrid.Last();
        FVector &last = maxCol.Last();
        return last;
    }
    return FVector(0, 0, 0);
}




bool FMeshedPolygon::GridValid() const{
    if(flagGrid.Num() == positionGrid.Num()){
        if(flagGrid.Num() > 0){
            return flagGrid[0].Num() == positionGrid[0].Num();
        }
    }
    return false;
}

void FMeshedPolygon::Init(TArray<FVector> &polygon, float widthOfInsideStep){
    if(polygon.Num() <= 0){
        return;
    }
    TArray<FVector> polygonRasterized = polygon;

    CurveRasterizer rasterizer;
    rasterizer.RasterizeVerteciesTo(polygonRasterized, widthOfInsideStep); // roght off rasterized edge
   
    
    
    stepSizeSaved = widthOfInsideStep;
    FindBounds(polygonRasterized);
    GenerateGrid();

    if(IsValid()){
        FlagTrue(polygonRasterized);
        FlagBetweenSpaceTrue();
    }
}

void FMeshedPolygon::InitForceSizeMin(TArray<FVector> &polygon, float widthOfInsideStep){
    if(!InitAsSinglePixel(polygon, widthOfInsideStep)){
        Init(polygon, widthOfInsideStep);
    }
}

bool FMeshedPolygon::InitAsSinglePixel(TArray<FVector> &polygon, float widthOfInsideStep){
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
        flagGrid[0][0] = true;

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

FVector FMeshedPolygon::center(TArray<FVector> &polygon){
    FVector sum(0,0,0);
    int count = polygon.Num(); 
    if(count > 0){
        for(const FVector &vertex : polygon){
            sum += vertex;
        }
        sum /= count;
    }
    return sum;
}

void FMeshedPolygon::FindBounds(
    TArray<FVector> &polygon
){
    BoundingBoxSimple box(polygon);
    FindBounds(box.bottomLeftNearVertex(), box.topRightFarVertex(), false);

    //debug size grid (looks ok, bool flags bricked)
    int x, y = 0;
    GetSizeGrid(x, y);
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedPolygon::sizeGrid (%d, %d)"), x, y
        )
    );
}

void FMeshedPolygon::FindBounds(FVector bottomLeft, FVector topRight, bool safeCheck){
    //safe bounds
    if(safeCheck){
        BoundingBoxSimple box;
        box.updateBoundsIfNeeded(bottomLeft);
        box.updateBoundsIfNeeded(topRight);

        minSaved = box.bottomLeftNearVertex();
        maxSaved = box.topRightFarVertex();
        return;
    }
    minSaved = bottomLeft;
    maxSaved = topRight;
}



void FMeshedPolygon::GenerateGrid(){
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

void FMeshedPolygon::GetSizeGrid(int &x, int &y){
    GetSizeGrid(x, y, stepSizeSaved);
}

void FMeshedPolygon::GetSizeGrid(int &x, int &y, float widthOfInsideStep){
    x = (maxSaved.X - minSaved.X) / widthOfInsideStep;
    y = (maxSaved.Y - minSaved.Y) / widthOfInsideStep;
}

int FMeshedPolygon::sizeX(){
    return flagGrid.Num();
}

int FMeshedPolygon::sizeY(){
    if(sizeX() > 0){
        return flagGrid[0].Num();
    }
    return 0;
}

void FMeshedPolygon::FlagTrue(const TArray<FVector> &polygon){
    for (int i = 0; i < polygon.Num(); i++){
        //deprecated, since rasterized spline might not be fully 
        //connected in step size but larger
        //FlagTrue(polygon[i]);

        int nextIndex = (i + 1) % polygon.Num();
        FlagTrueInterpolate(polygon[i], polygon[nextIndex]);
    }
}

//in between apply.
void FMeshedPolygon::FlagTrueInterpolate(
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
void FMeshedPolygon::GenerateStepDirectionForInterpolation(
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
void FMeshedPolygon::FlagTrue(const FVector &pos){
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

void FMeshedPolygon::FlagTruePolygonEdge(const FVector &pos){
    if(!IsValid()){
        return;
    }

    int x, y = 0;
    ToIndexBounded(pos, x, y);
    flagGrid[x][y] = true;

    std::pair<int, int> posCreated(x, y);
    if(!edgeIndices.Contains(posCreated)){
        edgeIndices.Add(posCreated);
    }
}

//once two bool flags are found, the space inbetween is marked true
void FMeshedPolygon::FlagBetweenSpaceTrue(){
    for (int i = 0; i < flagGrid.Num(); i++){
        TArray<bool> &flagBuffer = flagGrid[i];
        FlagBetweenSpaceTrue(flagBuffer);
    }
}


void FMeshedPolygon::FlagBetweenSpaceTrue(TArray<bool> &flagBuffer){
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

void FMeshedPolygon::FlagBetweenSpaceTrue(TArray<bool> &flagBuffer, int i, int j){
    i = std::max(i, 0);
    j = std::min(j, flagBuffer.Num());
    for (int k = i; k < j; k++){
        flagBuffer[k] = true;
    }
}

//bounded is needed since true flag is sometimes bricked otherwise
//because of floating point prescision errors, leading to polygon edges being OOB
void FMeshedPolygon::ToIndexBounded(const FVector &pos, int &x, int &y){
    ToIndexRaw(pos, x, y);

    x = std::max(0, x);
    y = std::max(0, y);
    
    x = std::min(x, flagGrid.Num() - 1);
    if(x >= 0 && x < flagGrid.Num()){
        y = std::min(y, flagGrid[x].Num() - 1);
    }
}

void FMeshedPolygon::ToIndexRaw(const FVector &pos, int &x, int &y){
    FVector relative = pos - minSaved; //AB = B - A
    x = FMath::RoundToInt(relative.X / stepSizeSaved);
    y = FMath::RoundToInt(relative.Y / stepSizeSaved);
}

bool FMeshedPolygon::IsInBound(const FVector &pos){
    int x = -1;
    int y = -1;
    return IsInBound(pos, x, y);
}

bool FMeshedPolygon::IsInBound(const FVector &pos, int &x, int &y){
    ToIndexRaw(pos, x, y);
    if(x >= 0 && x < flagGrid.Num()){
        if (y >= 0 && y < flagGrid[x].Num()){
            return true;
        }
    }
    return false;
}


bool FMeshedPolygon::FlagAt(int x, int y){
    if(x >= 0 && x < flagGrid.Num()){
        if (y >= 0 && y < flagGrid[x].Num()){
            return flagGrid[x][y];
        }
    }
    return false;
}

bool FMeshedPolygon::FlagAtPosition(const FVector &pos){
    int x = 0;
    int y = 0;
    ToIndexBounded(pos, x, y);
    return FlagAt(x, y);
}

bool FMeshedPolygon::PositionAtFlag(int x, int y, FVector &outPos){
    if(FlagAt(x,y)){
        return PositionAt(x, y, outPos);
    }
    return false;
}

bool FMeshedPolygon::PositionAt(const std::pair<int,int> &pair, FVector &outPos){
    return PositionAt(pair.first, pair.second, outPos);
}

bool FMeshedPolygon::PositionAt(int x, int y, FVector &outPos){
    if(x >= 0 && x < positionGrid.Num()){
        if (y >= 0 &&y < positionGrid[x].Num()){
            outPos = positionGrid[x][y];
            return true;
        }
    }
    return false;
}

FVector FMeshedPolygon::GetPositionAt(const std::pair<int,int> &pair){
    //copy
    FVector returned(0, 0, 0);
    PositionAt(pair, returned);
    return returned;
}


// ------ DEBUG APPEND MESH DATA -------



void FMeshedPolygon::AppendMeshedSurface(MeshData &data){
    if(!GridValid()){
        return;
    }
    for (int i = 0; i < flagGrid.Num() - 1; i++){
        for(int j = 0; j < flagGrid[i].Num(); j++){
            AppendAt(i, j, data);
        }
    }
}

void FMeshedPolygon::AppendAt(int i, int j, MeshData &data){
    TArray<FVector> buffer = GetQuadOrTriangleAt(i, j);
    if(buffer.Num() == 3){
        data.appendEfficent(buffer[0], buffer[1], buffer[2]);
    }
    if(buffer.Num() == 4){
        data.appendEfficent(buffer[0], buffer[1], buffer[2], buffer[3]);
    }
}

TArray<FVector> FMeshedPolygon::GetQuadOrTriangleAt(int i, int j){
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



void FMeshedPolygon::SetFlag(
    const FVector &pos,
    bool flagIn
){
    int x = 0;
    int y = 0;
    ToIndexBounded(pos, x, y);
    SetFlag(x, y, flagIn);
}



void FMeshedPolygon::SetFlag(
    const TArray<std::pair<int, int>> &indexPositions, 
    bool flag
){
    for (int i = 0; i < indexPositions.Num(); i++){
        const std::pair<int, int> &current = indexPositions[i];
        SetFlag(current, flag);
    }
}

void FMeshedPolygon::SetFlag(
    const std::pair<int, int> &indexPos, 
    bool flag
){
    SetFlag(indexPos.first, indexPos.second, flag);
}

void FMeshedPolygon::SetFlag(int x, int y, bool flag){
    if (x >= 0 && x < flagGrid.Num())
    {
        if (y >= 0 && y < flagGrid[x].Num()){
            flagGrid[x][y] = flag;
        }
    }
}

void FMeshedPolygon::SetPosition(int x, int y, const FVector &pos){
    if (x >= 0 && x < positionGrid.Num())
    {
        if (y >= 0 && y < positionGrid[x].Num()){
            positionGrid[x][y] = pos;
        }
    }
}





/// polygon join from polygon set

void FMeshedPolygon::GenerateFrom(
    const std::vector<FMeshedPolygon *> &polygons,
    const FVector &capA, 
    const FVector &capB,
    float stepSizeIn
){
    if(polygons.size() == 0){
        return;
    }

    if(polygons.size() == 1){
        if(const FMeshedPolygon *first = polygons[0]){
            *this = *first;
            return;
        }
    }
    OverrideStepSize(stepSizeIn);

    //make grid
    FindBounds(capA, capB, true);
    GenerateGrid();

    //flag from ingoing polygon data true
    if(polygons.size() > 1){
        for (int i = 0; i < polygons.size(); i++){
            if(const FMeshedPolygon *current = polygons[i]){
                FlagTrueFrom(*current);
            }
        }
    }
}

void FMeshedPolygon::FlagTrueFrom(const FMeshedPolygon &other){
    if(other.GridValid()){
        for (int x = 0; x < other.flagGrid.Num(); x++){
            const TArray<bool> &gridFlagColumn = other.flagGrid[x];
            for (int y = 0; y < gridFlagColumn.Num(); y++){
                if(x < other.positionGrid.Num()){
                    const TArray<FVector> &gridPositionColumn = other.positionGrid[x];
                    FlagTrueFromBuffer(
                        gridFlagColumn,
                        gridPositionColumn
                    );
                }
            }
        }
    }
}

void FMeshedPolygon::FlagTrueFromBuffer(
    const TArray<bool> &flagBuffer,
    const TArray<FVector> &positions
){
    if(flagBuffer.Num() == positions.Num()){
        for (int i = 0; i < flagBuffer.Num(); i++){
            const FVector &pos = positions[i];
            //if flagged true and in bound: flag in this bitmap as true
            if(flagBuffer[i]){
                int x = -1;
                int y = -1;
                if (IsInBound(pos, x, y)){
                    SetFlag(x, y, true);
                    SetPosition(x, y, pos);
                }
            }
        }
    }
}

void FMeshedPolygon::OverrideStepSize(float sizeIn){
    stepSizeSaved = sizeIn;
}

void FMeshedPolygon::OverrideMinMax(FVector &minIn, FVector &maxIn){
    minSaved = minIn;
    maxSaved = maxIn;
}

void FMeshedPolygon::GetMinMax(FVector &minOut, FVector &maxOut){
    minOut = minSaved;
    maxOut = maxSaved;
}

TArray<TArray<bool>> &FMeshedPolygon::GetFlagGrid(){
    return flagGrid;
}
TArray<TArray<FVector>> &FMeshedPolygon::GetPositionGrid(){
    return positionGrid;
} 
float FMeshedPolygon::GetStepSizeSaved(){
    return stepSizeSaved;
}


