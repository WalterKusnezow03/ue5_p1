#include "MeshedPolygonTrajectoryLayered.h"
#include "DebugPlugin/DebugHelper.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "PathfinderNNExtension/Interface/VisionCone.h"

#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/GridColorizer.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"
#include "CoreMath/algorithm/Vector/PositionCluster.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/TrajectoryCollection.h"
#include "PolygonPlugin/Public/GridBase/Operator/ConvolutionOperatorGauss.h"

void FMeshedPolygonTrajectoryLayered::Reset(){
    //CreateOrClearViewGrid(); //from raytracable polygon
    CreateOrClearGroundTruthGrid();
    CreateOrClearTrajectoryGrid();
    CreateOrClearEnemyPositionGrid();

    //not needed, happens on collection from pathfinder.
    //ClearFlags();
    

    //DebugHelper::logMessage("numedgeDebug FMeshedPolygonTrajectoryLayered Reset");
}

bool FMeshedPolygonTrajectoryLayered::FlagAndTimeDataValid() const {
    return IsValid() && TimeGridIsValid();
}

//player trajectories
void FMeshedPolygonTrajectoryLayered::EmbedTrajectories(TArray<Trajectory> &trajectories){
    if(IsValid()){
        CreateOrClearTrajectoryGrid();
        
        //zeiten normalisieren von 0 bis 1

        for (int i = 0; i < trajectories.Num(); i++){
            OverrideTime(trajectories[i]);
        }

        EmbedConeFromTrajectories(trajectories);
    }
    else
    {
        DebugHelper::logMessage("FMeshedPolygonTrajectoryLayered::TimeMap Cant Embed Trajectories");
    }
}

//player movement cone from trajectories
void FMeshedPolygonTrajectoryLayered::EmbedConeFromTrajectories(
    TArray<Trajectory> &trajectories
){
    CreateOrClearTrajectoryConeGrid();
    if(TrajectoryConeGridIsValid()){
        FVector2D endDir, globalDir;
        if(trajectories.Num() > 0){
            FVector endPositon = trajectories.Last().GetPosition();
            if(
                TrajectoryCollection::EndDir(trajectories, endDir) && 
                TrajectoryCollection::GlobalDir(trajectories, globalDir)
            ){
                TraceConeOnGridBetweenDirections(
                    endPositon,
                    globalDir,
                    endDir,
                    trajectoryConePrecited
                );
                
                //Blur
                float sigma = 2.0f;
                int sizeMask = 5;
                ConvolutionOperatorGauss gaussian(sigma, sizeMask);
                gaussian.ApplyOperator(trajectoryConePrecited);
            }
        }


        
    }
    

    //global dir
    //ending dir
    //cone from both directions

    //TraceCone(pos, cone->GetLookDir(), cone->GetAngle(), grid);


    //movement speed (?)
}


bool FMeshedPolygonTrajectoryLayered::TrajectoryConeGridIsValid(){
    return TGridIsValidToFlagGrid<float>(trajectoryConePrecited);
}
void FMeshedPolygonTrajectoryLayered::CreateOrClearTrajectoryConeGrid(){
    TCreateOrClearGrid<float>(trajectoryConePrecited, 0.0f);
}


    




//ground truth position
void FMeshedPolygonTrajectoryLayered::EmbedResultPosition(FVector &position){
    if(IsValid()){
        CreateOrClearGroundTruthGrid();
        int outX, outY = 1;
        if(IsInBound(position, outX, outY)){
            //where player is: 1, else: 0
            GaussianGroundTruthGrid(outX, outY, 4, 2.0f);
            TOverrideValue<float>(groundTruthGrid, outX, outY, playerGroundTruthPeak);
        }
    }
}

void FMeshedPolygonTrajectoryLayered::GaussianGroundTruthGrid(int x, int y, int size, float sigma){
    GaussianOnGrid(x, y, size, sigma, groundTruthGrid);
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

//cant be removed since we cant blur where polygons are
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

            //if not flagged - polygon - but: IS inverted
            //bool flagged = !FlagAt(i, j); //flags are inverted on collection (in prediction task.). Do not change.
            bool flagged = FlagAt(i, j); //which means its not flagged.

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


void FMeshedPolygonTrajectoryLayered::CreateOrClearGroundTruthGrid(){
    float clearedValue = -1.0f;
    TCreateOrClearGrid<float>(groundTruthGrid, 0.0f);

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


void FMeshedPolygonTrajectoryLayered::ClearGroundTruthGrid(){
    TClearGrid<float>(groundTruthGrid, 0.0f);
}


bool FMeshedPolygonTrajectoryLayered::GroundTruthGridIsValid() const{
    if(FlagGridIsValid()){ //buffer size at least one
        if(groundTruthGrid.Num() == flagGrid.Num()){
            if(groundTruthGrid[0].Num() == flagGrid[0].Num()){
                return true;
            }
        }
    }
    //GroundTruthGridIsValid
    //groundTruthGrid
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

void FMeshedPolygonTrajectoryLayered::AppendTrajectoryConeMap(
    TArray<uint8> &buffer
){

    //AppendViewMap(buffer);
    //viewGrid
    if(!TrajectoryConeGridIsValid()){
        CreateOrClearTrajectoryConeGrid();
    }

    if(TrajectoryConeGridIsValid()){
        AppendFloatMapToBuffer(buffer, trajectoryConePrecited);
    }
    else
    {
        DebugHelper::logMessage("FMeshedPolygon::AppendTrajectoryConeMap Failed", trajectoryConePrecited.Num());
    }
}













void FMeshedPolygonTrajectoryLayered::AppendFlagMapAsFloat(TArray<uint8> &buffer) const {
    const TArray<TArray<uint8>> &refMap = GetFlagGridConst();
    TArray<TArray<float>> converted;
    Uint8FlagMapToFloat(refMap, converted);
    AppendFloatMapToBuffer(buffer, converted);
}


void FMeshedPolygonTrajectoryLayered::AppendResultMapAsFloat(TArray<uint8> &buffer) const{
    AppendFloatMapToBuffer(buffer, groundTruthGrid);
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
    if(!GroundTruthGridIsValid()){
        CreateOrClearGroundTruthGrid();
    }

    if(GroundTruthGridIsValid()){
        int x = groundTruthGrid.Num();
        int y = groundTruthGrid[0].Num();
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
    TCreateOrClearGrid<float>(enemyPositions, clearedValue);
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
        //after embedding all cones: blur
        ApplyGaussViewGrid();
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
        NormalizeHeatMapThroshold(0.5f);
        RemoveHeatMapBorder(4); //3 pixels
        CacheResultPositionsFromHeatMap();
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

void FMeshedPolygonTrajectoryLayered::RemoveHeatMapBorder(int sizeBorder){
    RemoveMapBorder(heatMap, sizeBorder, 0.0f);
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

//call after GenerateMapFromPredicitontBytes(...)



////// ----- POST PROCESS RESULT EXTRACTION -----

// so nicht hinreichend: was man braucht ist: 
// result positions die sichtbar sind von einem Standpunkt aus
// oder eben alle



void FMeshedPolygonTrajectoryLayered::CacheResultPositionsFromHeatMap(){
    extractedHeatMapResults.Empty();
    GenerateResultPositions(extractedHeatMapResults);
    DebugHelper::logMessage("extractedHeatMapResults ", extractedHeatMapResults.Num());
}



void FMeshedPolygonTrajectoryLayered::GenerateResultPositions(
    TArray<FVector> &outpositions
){
    for (int i = 0; i < heatMap.Num(); i++)
    {
        TArray<float> &column = heatMap[i];
        for (int j = 0; j < column.Num(); j++){
            if(column[j] >= 0.8f){
                FVector posFromIndex = PositionFromIndex(i, j); //FMeshedPolygon::
                outpositions.Add(posFromIndex);
            }
        }
    }
}

//validates prediction on 2D raycasts through the polygon map
void FMeshedPolygonTrajectoryLayered::NotifyVisiblePositionsFor(
    IPathfinderNNInterface *interfaceIn
){
    if(interfaceIn){
        TArray<FVector> generated;
        GenerateResultPositionsVisibleBy(interfaceIn, generated);

        float distCluster = stepSizeSaved * 3.0f;
        PositionCluster clusterTool;
        clusterTool.ClusterPositions(generated, distCluster);
        
        if(generated.Num() > 0){
            interfaceIn->ResponseNNPositions(generated);
        }
    }
}





void FMeshedPolygonTrajectoryLayered::GenerateResultPositionsVisibleBy(
    IPathfinderNNInterface *interfaceIn,
    TArray<FVector> &outpositions
){
    if(interfaceIn){
        FVector pos = interfaceIn->GetWorldLocation();
        GenerateResultPositionsVisibleBy(pos, outpositions);
    }
}


void FMeshedPolygonTrajectoryLayered::GenerateResultPositionsVisibleBy(
    const FVector &lookFromPos,
    TArray<FVector> &outpositions
){
    GenerateResultPositionsVisibleBy(
        lookFromPos,
        extractedHeatMapResults,
        outpositions
    );
}


void FMeshedPolygonTrajectoryLayered::GenerateResultPositionsVisibleBy(
    const FVector &lookFromPos,
    TArray<FVector> &possibleSolutions,
    TArray<FVector> &outpositions
){
    for (int i = 0; i < possibleSolutions.Num(); i++){
        const FVector &solution = possibleSolutions[i];
        if(IsVisible(solution, lookFromPos)){
            outpositions.Add(solution);
        }
    }
}

// ---> TODO: Pair aus position und AActor der Requested: check visibility
// Position wird schonabgeaimt ja nein, dann belegen.
// nur da wo der peak am höhsten ist
// positionen clustern?

////// ----- POST PROCESS RESULT EXTRACTION -----


















////// ----- POST PROCESS IMAGE RESULT GENERATION ------

void FMeshedPolygonTrajectoryLayered::ColoredHeatMap(
    Image &image,
    FMeshedPolygonColorAttributes &attributes
){
    ColoredHeatMap(
        image,
        attributes.ColorMinHeat(),
        attributes.ColorMaxHeat(),
        attributes.ColorPolygon(),
        attributes.ColorView(),
        attributes.ColorTrajectory(),
        attributes.ColorPlayerResult()
    );
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

    //polygon layer
    colorizer.ColorizeFromUintFlag(image, flagGrid, colorPolygonFlagged, flagsInverted); 

    //view layer
    colorizer.ColorizeFromEpsilonFlagMix(image, viewGrid, 0.0f, true, colorViewGrid, 0.5f);
    
    //tracjetory override
    //colorizer.ColorizeFromEpsilonFlag(image, timeGrid, 0.0f, true, colorTrjacetory);

    colorizer.ColorizeFromEpsilonFlagMix(image, timeGrid, 0.0f, true, colorTrjacetory, 0.9f);
       
    colorizer.ColorizeFromEpsilonFlagMix(image, trajectoryConePrecited, 0.0f, true, colorTrjacetory, 0.2f);
    

    
    //player pos override
    colorizer.ColorizeFromEpsilonFlagMix(image, groundTruthGrid, 0.0f,true, playerPosResult, 0.9f);

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
        if(!GroundTruthGridIsValid()){
            CreateOrClearGroundTruthGrid();
        }
        if(!ViewGridValid()){
            CreateOrClearViewGrid();
        }
        if(!TrajectoryConeGridIsValid()){
            CreateOrClearTrajectoryConeGrid();
        }

        //append other data here
        TemplateBufferStorageInterface::TAppendGrid<float>(timeGrid, buffer);
        TemplateBufferStorageInterface::TAppendGrid<float>(groundTruthGrid, buffer);
        TemplateBufferStorageInterface::TAppendGrid<float>(viewGrid, buffer);
        TemplateBufferStorageInterface::TAppendGrid<float>(trajectoryConePrecited, buffer);
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
            TemplateBufferStorageInterface::TLoadGrid<float>(groundTruthGrid, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadGrid<float>(viewGrid, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadGrid<float>(trajectoryConePrecited, Ptr);
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
        AppendTrajectoryConeMap(buffer);

        return true;
    }
    return false;
}

bool FMeshedPolygonTrajectoryLayered::PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer){
    //sample and ground truth valid
    if(FlagAndTimeDataValid() && GroundTruthGridIsValid()){
        if(PrepareAppendRequestBinary(buffer)){
            AppendResultMapAsFloat(buffer);
            return true;
        }
    }
    return false;
}

// polygonDataCache.AppendResultMapAsFloat(resultbytes);
