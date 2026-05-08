#include "MeshedPolygonTrajectoryLayered.h"
#include "DebugPlugin/DebugHelper.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"


void FMeshedPolygonTrajectoryLayered::Reset(){
    CreateOrClearResultGrid();
    CreateOrClearTrajectoryGrid();
    ClearFlags();
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
                float value = expf(-dist2 / twoSigma2);
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
//void FMeshedPolygonTrajectoryLayered::OverrideTimeGaussian(int i, int j, float time, )




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






void FMeshedPolygonTrajectoryLayered::GenerateMapFromResultBytes(const TArray<uint8> &buffer){
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
    FColor colorTrjacetory,
    FColor playerPosResult
){
    //heat map
    ColoredHeatMap(image, colorMin, colorMax);

    //polygon override, inverted 
    for (int i = 0; i < flagGrid.Num(); i++){
        const TArray<uint8> &currentBuffer = flagGrid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            bool flag = currentBuffer[j];
            if(flagsInverted){
                flag = !flag;
            }

            if(flag){
                image.SetPixel(i, j, colorPolygonFlagged);
            }
        }
    }

    //tracjetory override
    for (int i = 0; i < timeGrid.Num(); i++){
        const TArray<float> &currentBuffer = timeGrid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            if(std::abs(currentBuffer[j]) > 0.0f){
                image.SetPixel(i, j, colorTrjacetory);
            }
        }
    }

    //player pos override
    for (int i = 0; i < resultGrid.Num(); i++){
        const TArray<float> &currentBuffer = resultGrid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            float scalar = currentBuffer[j];
            if(scalar > 0.0f){
                image.SetPixel(i, j, playerPosResult);
                /*FColor copy = playerPosResult;
                copy.R *= scalar;
                copy.G *= scalar;
                copy.B *= scalar;
                image.AddPixel(i, j, copy);*/
            }
        }
    }

    //needed.
    image.Transpose();
    image.FlipX();
    image.SetAlpha(255);
}

void FMeshedPolygonTrajectoryLayered::ColoredHeatMap(
    Image &image,
    FColor colorMin, 
    FColor colorMax
){
    int sizeX = heatMap.Num();
    if(heatMap.Num() > 0){
        FColor colorDir = DirColor(colorMin, colorMax);
        int sizeY = heatMap[0].Num();
        image.Setup(sizeX, sizeY);

        DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMeshedPolygonTrajectoryLayered::ColoredHeatMap %d %d"),
                sizeX, 
                sizeY
            )
        );

        int indexColorbuffer = 0;
        for (int i = 0; i < heatMap.Num(); i++)
        {
            const TArray<float> &column = heatMap[i];
            for (int j = 0; j < column.Num(); j++){
                float scalar = column[j];
                image.SetPixel(i, j, LerpColor(colorMin, colorDir, scalar));
            }
        }
    }
}

FColor FMeshedPolygonTrajectoryLayered::DirColor(
    const FColor &colorMin, 
    const FColor &colorMax
){
    FColor result;
    result.R = colorMax.R - colorMin.R;
    result.G = colorMax.G - colorMin.G;
    result.B = colorMax.B - colorMin.B;
    return result;
}

FColor FMeshedPolygonTrajectoryLayered::LerpColor(
    const FColor &colorMin, 
    const FColor &colorDir,
    float scalar
){
    FColor result;
    result.R = colorMin.R + scalar * colorDir.R;
    result.G = colorMin.G + scalar * colorDir.G;
    result.B = colorMin.B + scalar * colorDir.B;

    uint8 zero = 0;
    uint8 maxVal = 255;
    result.R = std::max(result.R, zero);
    result.G = std::max(result.G, zero);
    result.B = std::max(result.B, zero);

    result.R = std::min(result.R, maxVal);
    result.G = std::min(result.G, maxVal);
    result.B = std::min(result.B, maxVal);

    result.A = 255;
    return result;
}




// --- binary generation for sample save retrain model ---

void FMeshedPolygonTrajectoryLayered::AppendAsBinary(
    TArray<uint8> &buffer
){
    if(IsValid()){
        FMeshedPolygon::AppendAsBinary(buffer);

        //append other data here
        TemplateBufferStorageInterface::TAppendGrid<float>(timeGrid, buffer);
        TemplateBufferStorageInterface::TAppendGrid<float>(resultGrid, buffer);
    }

}



bool FMeshedPolygonTrajectoryLayered::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *& Ptr //reference to a pointer. Pointer by reference.
){

    if(FMeshedPolygon::LoadFromBinary(buffer, Ptr)){
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadGrid<float>(timeGrid, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadGrid<float>(resultGrid, Ptr);
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
