#include "MeshedPolygon.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"
#include "DebugPlugin/DebugHelper.h"

#include "PolygonPlugin/Public/Polygons/rasterizer/CurveRasterizer.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"



bool FMeshedPolygon::IsValid() const {
    return GridValid();
}

/*
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
*/



bool FMeshedPolygon::GridValid() const{
    /*if(flagGrid.Num() == positionGrid.Num()){
        if(flagGrid.Num() > 0){
            return flagGrid[0].Num() == positionGrid[0].Num();
        }
    }
    return false;*/
    return FlagGridIsValid();
}

bool FMeshedPolygon::FlagGridIsValid() const {
    if(flagGrid.Num() > 0){
        if(flagGrid[0].Num() > 0){
            return true;
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
    FindBounds(polygonRasterized); //polygonRasterized
    GenerateGrid();

    if(IsValid()){
        FlagTrue(polygonRasterized);
        AppendIndexHull(polygon);
        FlagBetweenSpaceTrue();
    }
}


void FMeshedPolygon::AppendIndexHull(const TArray<FVector> &polygon){
    FMeshedPolygonHullIndices hull;
    AppendIndexHull(polygon, hull);
    edgeSet.Add(hull);
}

void FMeshedPolygon::AppendIndexHull(const TArray<FVector> &polygon, FMeshedPolygonHullIndices &hull){
    for (int i = 0; i < polygon.Num(); i++){
        FVector pos = polygon[i];
        
        //local space edge points
        int x, y = 0;
        ToIndexRaw(pos, x, y);
        hull.AddPosition(x, y);

        /*if(IsValid()){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("FMeshedPolygon::AppendIndex %d %d of size %d %d (min, %s, pos %s)"), x, y,
                    flagGrid.Num(), flagGrid[0].Num(),
                    *minSaved.ToString(),
                    *pos.ToString()
                )
            );
            
        }*/
        
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
        flagGrid[0][0] = FlagAsInt8(true); //(uint8)1; // true;

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
    TCreateOrClearGrid<uint8>(flagGrid, FlagAsInt8(false));
    ClearFlags();
    
    /*
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
    }*/
}

void FMeshedPolygon::ClearFlags(){
    /*for (int i = 0; i < flagGrid.Num(); i++){
        TArray<uint8> &col = flagGrid[i];
        for (int j = 0; j < col.Num(); j++){
            col[j] = FlagAsInt8(false); //(uint8)0; // false;
        }
    }*/
    TClearGrid<uint8>(flagGrid, FlagAsInt8(false));
    flagsInverted = false;
}


void FMeshedPolygon::ClearEdgeSet(){
    edgeSet.Clear();
}

void FMeshedPolygon::ClearFlagsAndEdgeSet(){
    ClearFlags();
    ClearEdgeSet();
}

void FMeshedPolygon::GetSizeGrid(int &x, int &y){
    GetSizeGrid(x, y, stepSizeSaved);
}

void FMeshedPolygon::GetSizeGrid(int &x, int &y, float widthOfInsideStep){
    float deltaX = (maxSaved.X - minSaved.X);
    float deltaY = (maxSaved.Y - minSaved.Y);

    x = deltaX / widthOfInsideStep;
    y = deltaY / widthOfInsideStep;

    x = std::abs(x);
    y = std::abs(y);
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
    flagGrid[x][y] = FlagAsInt8(true); //(uint8)1; // true;

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
    flagGrid[x][y] = FlagAsInt8(true); //(uint8)1; // true;




    //deprecated here!
    //edgeIndices.AddPosition(x, y);

    
}






//once two bool flags are found, the space inbetween is marked true
void FMeshedPolygon::FlagBetweenSpaceTrue(){    
    for (int i = 0; i < flagGrid.Num(); i++){
        TArray<uint8> &flagBuffer = flagGrid[i];
        FlagBetweenSpaceTrue(flagBuffer);
    }
}


void FMeshedPolygon::FlagBetweenSpaceTrue(TArray<uint8> &flagBuffer){
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

void FMeshedPolygon::FlagBetweenSpaceTrue(TArray<uint8> &flagBuffer, int i, int j){
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
    x = FMath::FloorToInt(relative.X / stepSizeSaved);
    y = FMath::FloorToInt(relative.Y / stepSizeSaved);
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


void FMeshedPolygon::ToIndexRaw(const FMeshedPolygon &other, int &x, int &y){
    ToIndexRaw(other.minSaved, x, y);
}







bool FMeshedPolygon::FlagAt(int x, int y){
    if(x >= 0 && x < flagGrid.Num()){
        if (y >= 0 && y < flagGrid[x].Num()){
            return flagGrid[x][y] == FlagAsInt8(true); //(uint8) 1;
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


void FMeshedPolygon::InvertFlagMap(){
    for (int i = 0; i < flagGrid.Num(); i++){
        TArray<uint8> &column = flagGrid[i];
        for (int j = 0; j < column.Num(); j++){
            InvertFlag(column[j]);
        }
    }
    flagsInverted = !flagsInverted; //track inversion state
}

void FMeshedPolygon::InvertFlag(uint8 &flagCurrent){
    if(flagCurrent == FlagAsInt8(true)){
        flagCurrent = FlagAsInt8(false);
        return;
    }
    if(flagCurrent == FlagAsInt8(false)){
        flagCurrent = FlagAsInt8(true);
        return;
    }
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
    SetFlagInt(x, y, FlagAsInt8(flag));
}

uint8 FMeshedPolygon::FlagAsInt8(bool flag){
    uint8 flagAsInt = flag ? (uint8)1 : (uint8)0;
    return flagAsInt;
}

void FMeshedPolygon::SetFlagInt(int x, int y, uint8 flag){
    if (x >= 0 && x < flagGrid.Num())
    {
        if (y >= 0 && y < flagGrid[x].Num()){
            flagGrid[x][y] = flag;
        }
    }
}

/*
void FMeshedPolygon::SetPosition(int x, int y, const FVector &pos){
    if (x >= 0 && x < positionGrid.Num())
    {
        if (y >= 0 && y < positionGrid[x].Num()){
            positionGrid[x][y] = pos;
        }
    }
}*/





/// polygon join from polygon set

void FMeshedPolygon::GenerateFrom(
    const std::vector<FMeshedPolygon *> &polygons,
    const FVector &capA, 
    const FVector &capB,
    float stepSizeIn
){
    ClearFlagsAndEdgeSet();
    FVector relative = capB - capA; // AB = B - A
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedPolygon::GenerateFrom size %.2f %.2f, polygons %d"),
            relative.X,
            relative.Y,
            polygons.size()
        )
    );

    OverrideStepSize(stepSizeIn);

    //make grid
    FindBounds(capA, capB, false);
    GenerateGrid();

    if(IsValid()){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMeshedPolygon::GenerateFrom %d %d"),
                flagGrid.Num(),
                flagGrid[0].Num()
            )
        );
    }else{
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMeshedPolygon::GenerateFrom INVALID ")
            )
        );
        return;
    }

    //flag from ingoing polygon data true
    if(polygons.size() > 1){
        for (int i = 0; i < polygons.size(); i++){
            if(const FMeshedPolygon *current = polygons[i]){
                FlagTrueFromFastAndAppendLocalHulls(*current);
            }
        }
    }
}

void FMeshedPolygon::FlagTrueFromFastAndAppendLocalHulls(
    const FMeshedPolygon &other
){
    int xInGrid, yInGrid = 0;
    ToIndexRaw(other, xInGrid, yInGrid);

    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("FMeshedPolygon::Embed index(%d %d) bottomLeft %s, other bottomleft %s"),
            xInGrid,
            yInGrid,
            *BottomLeft().ToString(),
            *other.minSaved.ToString()
        )
    );*/

    AppendLocalHulls(xInGrid, yInGrid, other);
    FlagTrueFromFast(xInGrid, yInGrid, other);
}

void FMeshedPolygon::AppendLocalHulls(
    const int xStart,
    const int yStart,
    const FMeshedPolygon &other
){
    //add the offset to move the edge data to the correct space
    edgeSet.AddSetWithOffset(other.edgeSet, xStart, yStart); // + +
}



//expects same step size on polygon!
void FMeshedPolygon::FlagTrueFromFast(
    const int xStart,
    const int yStart,
    const FMeshedPolygon &other
){
    const TArray<TArray<uint8>> &otherFlagGrid = other.GetFlagGridConst();
    FlagTrueFromFast(xStart, yStart, otherFlagGrid);
}


void FMeshedPolygon::FlagTrueFromFast(
    const int xStart,
    const int yStart,
    const TArray<TArray<uint8>> &flagGridIn
){
    //move positions to relative space
    //bound check 
    //bit wise merge of uint8, 0 or 1 machts auch.
    for (int i = 0; i < flagGridIn.Num(); i++){
        FlagTrueFromFast(xStart, yStart, i, flagGridIn[i]);
    }
}



void FMeshedPolygon::FlagTrueFromFast(
    const int xStart,
    const int yStart,
    const int xOffset,
    const TArray<uint8> &flagBuffer //column
){
    int iMoved = xStart + xOffset; //move to correct column 
    

    //check in bound,
    //paste
    for(int j = 0; j < flagBuffer.Num(); j++){
        int jMoved = yStart + j;

        //flag if valid index
        //or operation is very fast
        if(FlagIndexInBound(iMoved, jMoved)){
            uint8 old = flagGrid[iMoved][jMoved];
            uint8 compare = flagBuffer[j];
            //bit wise or: should be fine since 
            //[0000 0001]
            //[0000 0000] or= 1

            //[0000 0000]
            //[0000 0000] or= 0

            flagGrid[iMoved][jMoved] = old || compare; 
        }
    }
}


bool FMeshedPolygon::FlagIndexInBound(const int x, const int y){
    if(x >= 0 && x < flagGrid.Num()){
        if(y >= 0 && y < flagGrid[x].Num()){
            return true;
        }
    }
    return false;
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

TArray<TArray<uint8>> &FMeshedPolygon::GetFlagGrid(){
    return flagGrid;
}

float FMeshedPolygon::GetStepSizeSaved() const{
    return stepSizeSaved;
}

const TArray<TArray<uint8>> &FMeshedPolygon::GetFlagGridConst() const {
    return flagGrid;
}

/*
const TArray<TArray<FVector>> &FMeshedPolygon::GetPositionGrid() const {
    return positionGrid;
}

TArray<TArray<FVector>> &FMeshedPolygon::GetPositionGrid(){
    return positionGrid;
} */




void FMeshedPolygon::GenerateColorBitmap(
    TArray<FColor> &outBuffer,
    FColor &free,
    FColor &blocked,
    int &resXOut,
    int &resYOut
)const{
    if(IsValid()){
        for (int i = 0; i < flagGrid.Num(); i++){
            const TArray<uint8> &currentBuffer = flagGrid[i];
            int offset = outBuffer.Num();
            outBuffer.SetNum(outBuffer.Num() + currentBuffer.Num());
            for (int j = 0; j < currentBuffer.Num(); j++){
                FColor made = currentBuffer[j] ? free : blocked;
                outBuffer[offset + j] = made;
            }
        }
        GetResolution(resXOut, resYOut);
    }
}

void FMeshedPolygon::GetResolution(int &xOut, int &yOut) const {
    if(IsValid()){
        xOut = flagGrid.Num();
        yOut = flagGrid[0].Num();
    }
}





void FMeshedPolygon::AppendFlagMap(
    TArray<uint8> &buffer
)const{
    //expects grid to be valid in size
    if(FlagGridIsValid()){
        for (int i = 0; i < flagGrid.Num(); i++){
            const TArray<uint8> &ref = flagGrid[i];

            int32 prevSize = buffer.Num();
            buffer.SetNumUninitialized(prevSize + ref.Num());
            uint8 *destPtr = buffer.GetData() + prevSize;
            
            /*
            FMemory::Memcpy( 
                void* Dest,
                const void* Src,
                SIZE_T Count
            )
            */
            FMemory::Memcpy( 
                destPtr,
                ref.GetData(),
                ref.Num() * sizeof(uint8)
            );

        }
    }else{
        DebugHelper::logMessage("FMeshedPolygon::AppendFlagMap Failed", flagGrid.Num());
    }
}








/// append as binary

void FMeshedPolygon::AppendAsBinary(
    TArray<uint8> &buffer
){
    if(IsValid()){
        TArray<TArray<uint8>> &flags = GetFlagGrid();
        FVector min, max;
        GetMinMax(min, max);
        
        //save polygon binary as
        //[stepSize][min,max][boolFlags][positions]
        TemplateBufferStorageInterface::TAppendSingleValue<float>(stepSizeSaved, buffer);
        TemplateBufferStorageInterface::TAppendSingleValue<FVector>(min, buffer);
        TemplateBufferStorageInterface::TAppendSingleValue<FVector>(max, buffer);
        TemplateBufferStorageInterface::TAppendGrid<uint8>(flags, buffer);
        edgeSet.AppendAsBinary(buffer);
    }
}



bool FMeshedPolygon::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *& Ptr //reference to a pointer. Pointer by reference.
){

    if(TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
        return false;
    }

    TArray<TArray<uint8>> &flags = GetFlagGrid();
    TemplateBufferStorageInterface::TLoadSingleValue<float>(stepSizeSaved, Ptr);
    TemplateBufferStorageInterface::TLoadSingleValue<FVector>(minSaved, Ptr);
    TemplateBufferStorageInterface::TLoadSingleValue<FVector>(maxSaved, Ptr);
    TemplateBufferStorageInterface::TLoadGrid<uint8>(flags, Ptr);
    edgeSet.LoadFromBinary(buffer, Ptr);
    return true;

}



int FMeshedPolygon::NumEdges(){
    return edgeSet.NumEdges();
}