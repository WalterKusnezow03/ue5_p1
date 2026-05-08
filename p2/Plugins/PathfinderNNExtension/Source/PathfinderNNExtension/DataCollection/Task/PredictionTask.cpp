#include "PredictionTask.h"



void PredictionTask::Reset(){
    trackedActorPtr = nullptr;
    taskStarted = false;
    taskCompleted = true;
    polygonDataCache.Reset();
}

void PredictionTask::Setup(ActorTrajectoryTracker *trackedActor){
    trackedActorPtr = trackedActor;
}

bool PredictionTask::WaitingForGroundTruth(){
    return taskStarted;
}
bool PredictionTask::TaskCompleted(){
    return !WaitingForGroundTruth() && taskCompleted;
}

bool PredictionTask::IsValid(){
    return trackedActorPtr != nullptr;
}

FMeshedPolygonTrajectoryLayered &PredictionTask::GetPolygonData(){
    return polygonDataCache;
}

int PredictionTask::ResultGridSizeBytes(){
    return polygonDataCache.ResultGridSizeBytes();
}

void PredictionTask::PrepareRequestBinary(TArray<uint8> &buffer){
    taskStarted = true;
    taskCompleted = false;

    PrepareRequestMap(polygonDataCache);

    if(!polygonDataCache.PrepareAppendRequestBinary(buffer)){
        buffer.Empty();
        Reset();
        return;
    }





    // --- deprecated ---

    /*
    //WRITE BINARY
    //polygon.AppendFlagMap(buffer); //deprecated!
    polygonDataCache.AppendFlagMapAsFloat(buffer);
    polygonDataCache.AppendTimeMap(buffer);

    //reset if data not valid!
    if(!polygonDataCache.FlagAndTimeDataValid()){
        buffer.Empty();
        Reset();
        return;
    }*/

    //WriteData(buffer);
}

void PredictionTask::PrepareRequestMap(FMeshedPolygonTrajectoryLayered &polygonData){
    if(trackedActorPtr){
        polygonData.ClearFlags(); //clear previous map


        FVector location = trackedActorPtr->ActorLocation();

        DebugHelper::logMessage("PredictionTask::PrepareRequestMap ", location);

        //copy location for real result generation
        locationOfRequest = location;

        

        //FMeshedPolygonTrajectoryLayered polygonData;
        polygonProxy.CollectPolygon(
            location,
            radiusMeter * 100.0f,
            polygonData
        );

        //invert flag map for 1 possible position and 0 not possible
        //might be better for training
        polygonData.InvertFlagMap();// ---> lags.

        //move trajectory times to space of -1..1 to be similar to binary occupancy map of polygons!
        TArray<Trajectory> trajectories = trackedActorPtr->worldTrajectoriesNormalizedTime(); 
        // trackedActorPtr->worldTrajectories();
        polygonData.EmbedTrajectories(trajectories);
    }
}


void PredictionTask::GenerateResultPositions(
    TArray<FVector> &positions
){
    polygonDataCache.GenerateResultPositions(positions);
}

//prepares result binary!
bool PredictionTask::TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes){
    
    if(IsValid()){
        DebugHelper::showScreenMessage("PredictionTask::TickTask Z", FColor::Blue);
        //if(taskStarted && !taskCompleted){ //new condition
        if(taskStarted){ //new condition
            DebugHelper::showScreenMessage("PredictionTask::TickTask A", FColor::Blue);
            if(trackedActorPtr->IsFlaggedVisibleReset()){
                //task is finished now, prepare bin for NN python / shared mem
                resultbytes.Empty();
                DebugHelper::showScreenMessage("PredictionTask::TickTask B", FColor::Blue);
                

                //embed player position into a other flag grid, same size as polygon data grid
                //add player pos single!
                //append to bytes
                FVector location = trackedActorPtr->ActorLocation();
                polygonDataCache.EmbedResultPosition(location);
                polygonDataCache.AppendResultMapAsFloat(resultbytes);

                //reset flag
                taskStarted = false;

                return true;
            }
        }
    

        
    }
    return false;
}
      
//Heat map prediction embedding
void PredictionTask::GenerateMapFromResultBytes(const TArray<uint8> &buffer){
    polygonDataCache.GenerateMapFromResultBytes(buffer);
}

bool PredictionTask::IsSameActor(AActor *actorCheck){
    if(actorCheck){
        if(trackedActorPtr){
            return trackedActorPtr->GetActor() == actorCheck;
        }
    }
    return false;
}


void PredictionTask::ColoredHeatMap(
    Image &image,
    FColor colorMin,
    FColor colorMax,
    FColor colorPolygonFlagged,
    FColor colorTrjacetory,
    FColor playerPosResult
){
    polygonDataCache.ColoredHeatMap(
        image, colorMin, colorMax, colorPolygonFlagged, colorTrjacetory, playerPosResult
    );
    taskCompleted = true;
}


