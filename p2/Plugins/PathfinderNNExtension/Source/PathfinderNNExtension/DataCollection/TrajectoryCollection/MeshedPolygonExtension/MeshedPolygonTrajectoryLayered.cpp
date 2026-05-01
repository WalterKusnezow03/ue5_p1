#include "MeshedPolygonTrajectoryLayered.h"
#include "DebugPlugin/DebugHelper.h"


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
        DebugHelper::logMessage(
            "FMeshedPolygonTrajectoryLayered::TimeMap Setup",
            timeGrid.Num()
        );

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
            //OverrideTime(outX, outY, time);
            TOverrideValue<uint8>(resultGrid, outX, outY, 100);
        }
    }
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
    /*if(i >= 0 && j >= 0 && i < timeGrid.Num() && j < timeGrid[i].Num()){
        timeGrid[i][j] = time;
    }
    */
    TOverrideValue<float>(timeGrid, i, j, time);
    if(false){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMeshedPolygonTrajectoryLayered::Time override %.2f"), timeGrid[i][j]
            )
        );
    }
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
    TCreateOrClearGrid<uint8>(resultGrid, 0);

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
    TClearGrid<uint8>(resultGrid, 0);
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
    const TArray<TArray<uint8>> &refMap = resultGrid;
    TArray<TArray<float>> converted;
    Uint8FlagMapToFloat(refMap, converted);
    AppendFloatMapToBuffer(buffer, converted);
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
        NormalizeHeatMap();
        //LogHeatMap("HeatMapNormalized: ");
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

    //polygon override
    for (int i = 0; i < flagGrid.Num(); i++){
        const TArray<uint8> &currentBuffer = flagGrid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            if(currentBuffer[j]){
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
        const TArray<uint8> &currentBuffer = resultGrid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            if(currentBuffer[j] > 0){
                image.SetPixel(i, j, playerPosResult);
                image.SetPixel(i+1, j, playerPosResult);
                image.SetPixel(i, j+1, playerPosResult);
                image.SetPixel(i-1, j, playerPosResult);
                image.SetPixel(i, j-1, playerPosResult);
            }
        }
    }
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
