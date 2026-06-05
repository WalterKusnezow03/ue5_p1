#include "MeshedPolygonTrajectoryLayered.h"
#include "DebugPlugin/DebugHelper.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "PathfinderNNExtension/Interface/VisionCone.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/GridColorizer.h"


void FMeshedPolygonTrajectoryLayered::Reset(){
    //CreateOrClearViewGrid(); //from raytracable polygon
    CreateOrClearResultGrid();
    CreateOrClearTrajectoryGrid();
    CreateOrClearEnemyPositionGrid();

    //not needed, happens on collection from pathfinder.
    //ClearFlags();
    

    //DebugHelper::logMessage("numedgeDebug FMeshedPolygonTrajectoryLayered Reset");
}

bool FMeshedPolygonTrajectoryLayered::FlagAndTimeDataValid() const {
    return IsValid() && TimeGridIsValid();
}

void FMeshedPolygonTrajectoryLayered::EmbedTrajectories(TArray<Trajectory> &trajectories){
    if(IsValid()){
        CreateOrClearTrajectoryGrid();
        
        //zeiten normalisieren von 0 bis 1

        for (int i = 0; i < trajectories.Num(); i++){
            OverrideTime(trajectories[i]);
        }
    }else{
        DebugHelper::logMessage("FMeshedPolygonTrajectoryLayered::TimeMap Cant Embed Trajectories");
    }
}

void FMeshedPolygonTrajectoryLayered::EmbedResultPosition(FVector &position){
    if(IsValid()){
        CreateOrClearResultGrid();
        int outX, outY = 1;
        if(IsInBound(position, outX, outY)){
            //where player is: 1, else: 0
            GaussianResultGrid(outX, outY, 4, 2.0f);
            TOverrideValue<float>(resultGrid, outX, outY, 1.0f);
        }
    }
}

void FMeshedPolygonTrajectoryLayered::GaussianResultGrid(int x, int y, int size, float sigma){
    GaussianOnGrid(x, y, size, sigma, resultGrid);
}

void FMeshedPolygonTrajectoryLayered::GaussianOnGrid(
    int x, 
    int y, 
    int size, 
    float sigma, 
    TArray<TArray<float>> &grid
){
    GaussianOnGrid(x, y, size, sigma, grid, 1.0f);
}

void FMeshedPolygonTrajectoryLayered::GaussianOnGrid(
    int x, 
    int y, 
    int size, 
    float sigma, 
    TArray<TArray<float>> &grid,
    float scaledValue 
){
    //conv operator
    const float twoSigma2 = 2.0f * sigma * sigma;
    for (int i = x - size; i < x + size; i++){
        for (int j = y - size; j < y + size; j++){
            //if not flagged - polygon - but: inverted
            //bool flagged = !FlagAt(i, j); //flags are inverted on collection (in prediction task.). Do not change.
            bool flagged = FlagAt(i, j);

            /*if(flagsInverted){
                flagged = !flagged;
            }*/

            if(flagged){
                float dx = float(i - x); //AB = B - A
                float dy = float(j - y);
                float dist2 = dx * dx + dy * dy; //size(dx,dy)^2
                float scale = expf(-dist2 / twoSigma2);
                float value = scaledValue * scale;
                TOverrideValue<float>(grid, i, j, value);
            }
        }
    }
}

float FMeshedPolygonTrajectoryLayered::Gaussian(
    int i, 
    int j, 
    int xCenter, 
    int yCenter, 
    float twoSigma2
){
    float dx = float(i - xCenter); //AB = B - A
    float dy = float(j - yCenter);
    float dist2 = dx * dx + dy * dy; //size(dx,dy)^2
    float value = expf(-dist2 / twoSigma2);
    return value;
}

void FMeshedPolygonTrajectoryLayered::OverrideTime(Trajectory &current){
    FVector pos = current.GetPosition();
    float time = current.GetTime(); //will be negative, time relative to 0.
    
    int outX, outY = 1;
    if(IsInBound(pos, outX, outY)){
        OverrideTime(outX, outY, time);

        //gaussian blob
        int size = 2; //small gaussian blob
        float sigma = 2.0f;
        OverrideTimeGaussian(outX, outY, time, size, sigma);
    }
}

void FMeshedPolygonTrajectoryLayered::OverrideTime(int i, int j, float time){
    TOverrideValue<float>(timeGrid, i, j, time);
    if(false){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMeshedPolygonTrajectoryLayered::Time override %.2f"), timeGrid[i][j]
            )
        );
    }
}


// TODO!
void FMeshedPolygonTrajectoryLayered::OverrideTimeGaussian(int i, int j, float time, int size, float sigma){
    GaussianOnGrid(i, j, size, sigma, timeGrid, time);
}




void FMeshedPolygonTrajectoryLayered::CreateOrClearTrajectoryGrid(){
    TCreateOrClearGrid<float>(timeGrid, 0.0f);
    
    /*//grid size from bool grid
    int x = sizeX();
    int y = sizeY();
    if(x > 0 && y > 0){
        if (TGridIsSize<float>(x, y, timeGrid)){
            ClearTimeGrid();
            return;
        }
        TGenerateGrid<float>(x, y, timeGrid);
        ClearTimeGrid();
    }*/
}


bool FMeshedPolygonTrajectoryLayered::TimeGridIsValid() const {
    if(FlagGridIsValid()){ //buffer size at least one
        if(timeGrid.Num() == flagGrid.Num()){
            if(timeGrid[0].Num() == flagGrid[0].Num()){
                return true;
            }
        }
    }
    return false;
}


void FMeshedPolygonTrajectoryLayered::CreateOrClearResultGrid(){
    float clearedValue = -1.0f;
    TCreateOrClearGrid<float>(resultGrid, 0.0f);

    /*//grid size from bool grid
    int x = sizeX();
    int y = sizeY();
    if(x > 0 && y > 0){
        if (TGridIsSize<uint8>(x, y, resultGrid)){
            ClearResultGrid();
            return;
        }
        TGenerateGrid<uint8>(x, y, resultGrid);
        ClearResultGrid();
    }*/
}


void FMeshedPolygonTrajectoryLayered::ClearResultGrid(){
    TClearGrid<float>(resultGrid, 0.0f);
}


bool FMeshedPolygonTrajectoryLayered::ResultGridIsValid() const{
    if(FlagGridIsValid()){ //buffer size at least one
        if(resultGrid.Num() == flagGrid.Num()){
            if(resultGrid[0].Num() == flagGrid[0].Num()){
                return true;
            }
        }
    }
    return false;
}




void FMeshedPolygonTrajectoryLayered::AppendTimeMap(
    TArray<uint8> &buffer
)const{
    //expects grid to be valid in size
    if(TimeGridIsValid()){
        AppendFloatMapToBuffer(buffer, timeGrid);
    }
    else
    {
        DebugHelper::logMessage("FMeshedPolygon::AppendTimeMap Failed", timeGrid.Num());
    }
}

void FMeshedPolygonTrajectoryLayered::AppendViewMap(
    TArray<uint8> &buffer
){

    //AppendViewMap(buffer);
    //viewGrid
    if(!ViewGridValid()){
        CreateOrClearViewGrid();
    }

    if(ViewGridValid()){
        AppendFloatMapToBuffer(buffer, viewGrid);
    }
    else
    {
        DebugHelper::logMessage("FMeshedPolygon::AppendViewMap Failed", viewGrid.Num());
    }
}




void FMeshedPolygonTrajectoryLayered::AppendFlagMapAsFloat(TArray<uint8> &buffer) const {
    const TArray<TArray<uint8>> &refMap = GetFlagGridConst();
    TArray<TArray<float>> converted;
    Uint8FlagMapToFloat(refMap, converted);
    AppendFloatMapToBuffer(buffer, converted);
}


void FMeshedPolygonTrajectoryLayered::AppendResultMapAsFloat(TArray<uint8> &buffer) const{
    AppendFloatMapToBuffer(buffer, resultGrid);
}






void FMeshedPolygonTrajectoryLayered::AppendFloatMapToBuffer(
    TArray<uint8> &buffer,
    const TArray<TArray<float>> &someMap
)const{
    for (int i = 0; i < someMap.Num(); i++){
        const TArray<float> &ref = someMap[i];

        int32 prevSize = buffer.Num();
        buffer.SetNumUninitialized(prevSize + ref.Num() * sizeof(float));
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
            ref.Num() * sizeof(float)
        );

    }
}


void FMeshedPolygonTrajectoryLayered::Uint8FlagMapToFloat(
    const TArray<TArray<uint8>> &refMap, 
    TArray<TArray<float>> &converted
) const {
    converted.SetNum(refMap.Num());
    for (int i = 0; i < refMap.Num(); i++){
        const TArray<uint8> &refMapCurrent = refMap[i];
        TArray<float> &current = converted[i];
        current.SetNumUninitialized(refMapCurrent.Num());
        for (int j = 0; j < current.Num(); j++){
            current[j] = refMapCurrent[j];
        }
    }
}





int FMeshedPolygonTrajectoryLayered::ResultGridSizeBytes(){
    if(!ResultGridIsValid()){
        CreateOrClearResultGrid();
    }

    if(ResultGridIsValid()){
        int x = resultGrid.Num();
        int y = resultGrid[0].Num();
        return x * y * sizeof(float); //as float instead of uint8.
    }
    return -1;
}


/*
TArray<TArray<float>> enemyPositions;
bool EnemyPositionGridIsValid();
void CreateOrClearEnemyPositionGrid();

*/

bool FMeshedPolygonTrajectoryLayered::EnemyPositionGridIsValid(){
    return TGridIsValidToFlagGrid<float>(enemyPositions);
}

void FMeshedPolygonTrajectoryLayered::CreateOrClearEnemyPositionGrid(){
    float clearedValue = 0.0f;
    TCreateOrClearGrid<float>(resultGrid, clearedValue);
}

void FMeshedPolygonTrajectoryLayered::EmbedEnemyPositions(const TArray<FVector> &enemies){
    if(IsValid()){

        DebugHelper::logMessage("FMeshedPolygonTrajectoryLayered::EmbedEnemyPositions ", enemies.Num());

        CreateOrClearEnemyPositionGrid();
        for (int i = 0; i < enemies.Num(); i++){
            const FVector &pos = enemies[i];
            EmbedEnemyPosition(pos);
        }
    }
}


void FMeshedPolygonTrajectoryLayered::EmbedEnemyPosition(const FVector &position){
    int outX = -1;
    int outY = -1;
    if(IsInBound(position, outX, outY)){
        //where enemy is: 1, else: 0
        GaussianOnGrid(outX, outY, 4, 2.0f, enemyPositions); //between 0 and 1
        TOverrideValue<float>(enemyPositions, outX, outY, 1.0f);
    }
}

void FMeshedPolygonTrajectoryLayered::EmbedEnemyVision(const TArray<FVisionCone *> &cones){
    if(IsValid()){
        CreateOrClearViewGrid();
        
        
        // void EmbedEnemyPositions(const Tarray<FVisionCone *> &cones);
        //DebugHelper::logMessage("FMeshedPolygonTrajectoryLayered::EmbedEnemyVision ", cones.Num());
        //DebugHelper::logMessage(FString::Printf(TEXT("numedgeDebug FMeshedPolygonTrajectoryLayered::EmbedEnemyVision num edges %d"), NumEdges()));

        for (int i = 0; i < cones.Num(); i++){
            if(FVisionCone *cone = cones[i]){
                const FVector &pos = cone->ActorLocation();
                EmbedEnemyPosition(pos);
                TraceCone(pos, cone->GetLookDir(), cone->GetAngle());
            }
        }
    }
}







/// from predicion change name!
void FMeshedPolygonTrajectoryLayered::GenerateMapFromPredicitontBytes(const TArray<uint8> &buffer){
    if(buffer.Num() > 0){
        TCreateOrClearGrid<float>(heatMap, 0.0f);

        //if(buffer.Num() <)

        const uint8 *Ptr = buffer.GetData();
        for (int i = 0; i < heatMap.Num(); i++){
            TArray<float> &column = heatMap[i];

            int bytesToCopy = column.Num() * sizeof(float);
            void *Dest = column.GetData();
            FMemory::Memcpy( 
                Dest,
                Ptr,
                bytesToCopy
            );
            Ptr += bytesToCopy;
        }
        //LogHeatMap("HeatMapRaw: ");
        //NormalizeHeatMap();
        NormalizeHeatMapThroshold(0.1f);
        // LogHeatMap("HeatMapNormalized: ");
    }
}

void FMeshedPolygonTrajectoryLayered::LogHeatMap(FString prefix){
    for (int i = 0; i < heatMap.Num(); i++){
        FString message = prefix;
        prefix += " [";
        const TArray<float> &column = heatMap[i];
        for (int j = 0; j < column.Num(); j++){
            prefix += FString::Printf(TEXT(" %.2f "), column[j]);
        }
        prefix += "]";
        DebugHelper::logMessage(message);
    }
}


void FMeshedPolygonTrajectoryLayered::NormalizeHeatMap(){
    float min = 0.0f;
    float max = 0.0f;
    for (int i = 0; i < heatMap.Num(); i++){
        const TArray<float> &column = heatMap[i];
        for (int j = 0; j < column.Num(); j++){
            min = std::min(column[j], min);
            max = std::max(column[j], max);
        }
    }
    float range = max - min; //AB = B - A
    DebugHelper::logMessage(FString::Printf(TEXT("FMeshedPolygonTrajectoryLayered::HeatMapRange %.2f"), range));
    range = std::max(range, 0.0001f);
    for (int i = 0; i < heatMap.Num(); i++)
    {
        TArray<float> &column = heatMap[i];
        for (int j = 0; j < column.Num(); j++){
            float xRelative = column[j] - min; //AB = B - A
            column[j] = xRelative / range;
        }
    }
}

void FMeshedPolygonTrajectoryLayered::NormalizeHeatMapThroshold(float capZero){
    NormalizeHeatMap();
    //debugging pruposes
    for (int i = 0; i < heatMap.Num(); i++){
        
        TArray<float> &column = heatMap[i];
        for (int j = 0; j < column.Num(); j++){
            if(column[j] < capZero){
                column[j] = 0.0f;
            }
        }
    }
}

void FMeshedPolygonTrajectoryLayered::GenerateResultPositions(
    TArray<FVector> &positions
){
    //// ---- TODO ----
}







void FMeshedPolygonTrajectoryLayered::ColoredHeatMap(
    Image &image,
    FColor colorMin, 
    FColor colorMax,
    FColor colorPolygonFlagged,
    FColor colorViewGrid,
    FColor colorTrjacetory,
    FColor playerPosResult
){
    //heat map
    FGridColorizer colorizer;
    colorizer.ColorizeFromLerp(image, heatMap, colorMin, colorMax); //heat map

    colorizer.ColorizeFromEpsilonFlag(image, viewGrid, 0.0f, true, colorViewGrid);

    //polygon layer
    colorizer.ColorizeFromUintFlag(image, flagGrid, colorPolygonFlagged, flagsInverted); 
    
    //tracjetory override
    colorizer.ColorizeFromEpsilonFlag(image, timeGrid, 0.0f, true, colorTrjacetory);
    
    //player pos override
    colorizer.ColorizeFromEpsilonFlag(image, resultGrid, 0.0f, playerPosResult);
    
    //vision cone override
    /*
    for (int i = 0; i < viewGrid.Num(); i++){
        const TArray<float> &currentBuffer = viewGrid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            if(currentBuffer[j] > 0.0f){
                if(FlagAt(i,j) == false){
                    image.SetPixel(i, j, colorViewGrid);
                }
            }
        }
    }*/
    

    //are correct
    /*colorizer.ColorizeEdgePoints(
        image,
        edgeSet,
        FColor::Black
    );*/

    //needed.
    image.Transpose();
    image.FlipX();
    image.SetAlpha(255);
}



// --- binary generation for sample save retrain model ---

void FMeshedPolygonTrajectoryLayered::AppendAsBinary(
    TArray<uint8> &buffer
){
    if(IsValid()){
        //FMeshedPolygon::AppendAsBinary(buffer);
        FMeshedPolygonRaytracable::AppendAsBinary(buffer);

        if(!TimeGridIsValid()){
            CreateOrClearTrajectoryGrid();
        }
        if(!ResultGridIsValid()){
            CreateOrClearResultGrid();
        }
        if(!ViewGridValid()){
            CreateOrClearViewGrid();
        }

        //append other data here
        TemplateBufferStorageInterface::TAppendGrid<float>(timeGrid, buffer);
        TemplateBufferStorageInterface::TAppendGrid<float>(resultGrid, buffer);
        TemplateBufferStorageInterface::TAppendGrid<float>(viewGrid, buffer);
    }

}



bool FMeshedPolygonTrajectoryLayered::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *& Ptr //reference to a pointer. Pointer by reference.
){

    //if(FMeshedPolygon::LoadFromBinary(buffer, Ptr)){
    if(FMeshedPolygonRaytracable::LoadFromBinary(buffer, Ptr)){
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadGrid<float>(timeGrid, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadGrid<float>(resultGrid, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadGrid<float>(viewGrid, Ptr);
        }
        return true;
    }
    return false;

}





///// ------ SIMPLIFIED ACCESS FOR NN BATCH PREPERATION -------
//request data only.
bool FMeshedPolygonTrajectoryLayered::PrepareAppendRequestBinary(TArray<uint8> &buffer){
    if(FlagAndTimeDataValid()){
        AppendFlagMapAsFloat(buffer);
        AppendTimeMap(buffer);
        AppendViewMap(buffer);
        
        return true;
    }
    return false;
}

bool FMeshedPolygonTrajectoryLayered::PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer){
    //sample and ground truth valid
    if(FlagAndTimeDataValid() && ResultGridIsValid()){
        if(PrepareAppendRequestBinary(buffer)){
            AppendResultMapAsFloat(buffer);
            return true;
        }
    }
    return false;
}

// polygonDataCache.AppendResultMapAsFloat(resultbytes);
