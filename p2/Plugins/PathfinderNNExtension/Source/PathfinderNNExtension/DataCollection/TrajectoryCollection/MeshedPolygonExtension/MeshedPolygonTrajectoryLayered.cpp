#include "MeshedPolygonTrajectoryLayered.h"
#include "DebugPlugin/DebugHelper.h"



void FMeshedPolygonTrajectoryLayered::EmbedTrajectories(TArray<Trajectory> &trajectories){
    if(IsValid()){
        CreateOrClearTrajectoryGrid();

        for (int i = 0; i < trajectories.Num(); i++){
            OverrideTime(trajectories[i]);
        }
    }
}

void FMeshedPolygonTrajectoryLayered::EmbedResultPosition(FVector &position){
    if(IsValid()){
        CreateOrClearResultGrid();
        int outX, outY = 1;
        if(IsInBound(position, outX, outY)){
            //OverrideTime(outX, outY, time);
            TOverrideValue<uint8>(resultGrid, outX, outY, 1);
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

void FMeshedPolygonTrajectoryLayered::ClearTimeGrid(){
    /*for (int i = 0; i < timeGrid.Num(); i++){
        TArray<float> &column = timeGrid[i];
        for (int j = 0; j < column.Num(); j++){
            column[j] = 0.0f;
        }
    }*/

    TClearGrid<float>(timeGrid, 0.0f);
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


