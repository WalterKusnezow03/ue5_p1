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

    //WRITE BINARY
    //polygon.AppendFlagMap(buffer); //deprecated!
    polygonDataCache.AppendFlagMapAsFloat(buffer);
    polygonDataCache.AppendTimeMap(buffer);

    if(!polygonDataCache.FlagAndTimeDataValid()){
        buffer.Empty();
        Reset();
        return;
    }

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

        TArray<Trajectory> trajectories = trackedActorPtr->worldTrajectories();
        polygonData.EmbedTrajectories(trajectories);
    }
}




//prepares result binary!
bool PredictionTask::TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes){
    if(IsValid()){
        //if(taskStarted && !taskCompleted){ //new condition
        if(taskStarted){ //new condition
            if(trackedActorPtr->IsFlaggedVisible()){
                //task is finished now, prepare bin for NN python / shared mem
                resultbytes.Empty();

                

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


