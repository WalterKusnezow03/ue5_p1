#include "PredictionTask.h"
#include "PathfinderNNExtension/Request/FPathFinderNNRequestPackage.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"


void PredictionTask::Reset(){
    trackedActorPtr = nullptr;
    taskStarted = false;
    taskCompleted = true;
    polygonDataCache.Reset();  //clear task polygon data
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
    return polygonDataCache.ResultGridSizeBytes(); //prediction / result byte size of grid
}

void PredictionTask::EmbedEnemyPositionsAndVision(const TArray<FVisionCone*> &enemies){
    polygonDataCache.EmbedEnemyVision(enemies);
}



void PredictionTask::EmbedEnemyPositionsAndVision(FPathFinderNNRequestPackage &queue){
    TArray<FVisionCone*> positionsToEmbed;
    queue.GetRequesterVisionCones(positionsToEmbed);
    EmbedEnemyPositionsAndVision(positionsToEmbed);
}


void PredictionTask::GenerateAndNotifyResultPositions(FPathFinderNNRequestPackage *queue){
    if(queue){
        TArray<IPathfinderNNInterface *> interfaces = queue->GetSubscribedActors();
        for (int i = 0; i < interfaces.Num(); i++){
            if(IPathfinderNNInterface *current = interfaces[i]){
                bool useVisibility = false;
                polygonDataCache.NotifyVisiblePositionsFor(current, useVisibility);
            }
        }
    }
}












void PredictionTask::PrepareRequestBinary(TArray<uint8> &buffer){
    taskStarted = true;
    taskCompleted = false;

    PrepareRequestMap(polygonDataCache);

    //DebugHelper::logMessage(FString::Printf(TEXT("numedgeDebug PredictionTask num edges %d"), polygonDataCache.NumEdges()));
    //400

    //if appending binary data failed: reset task and clean buffer
    if(!polygonDataCache.PrepareAppendRequestBinary(buffer)){
        buffer.Empty();
        Reset(); //clear task.
        return;
    }
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
        polygonData.ResizeGrid144();

        //invert flag map for 1 possible position and 0 not possible
        //might be better for training
        polygonData.InvertFlagMap();

        //move trajectory times to space of -1..1 to be similar to binary occupancy map of polygons!
        TArray<Trajectory> trajectories = trackedActorPtr->worldTrajectoriesNormalizedTime(); 
        // trackedActorPtr->worldTrajectories();
        polygonData.EmbedTrajectories(trajectories);
    }
}


//prepares result binary!
bool PredictionTask::TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes){
    
    if(IsValid()){
        //DebugHelper::showScreenMessage("PredictionTask::TickTask Z", FColor::Blue);
        //if(taskStarted && !taskCompleted){ //new condition
        if(taskStarted){ //new condition
            //DebugHelper::showScreenMessage("PredictionTask::TickTask A", FColor::Blue);
            if(trackedActorPtr->IsFlaggedVisibleReset()){
                //task is finished now, prepare bin for NN python / shared mem
                resultbytes.Empty();
                //DebugHelper::showScreenMessage("PredictionTask::TickTask B", FColor::Blue);
                

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
void PredictionTask::GenerateMapFromPredicitontBytes(const TArray<uint8> &buffer){
    polygonDataCache.GenerateMapFromPredicitontBytes(buffer);
}

void PredictionTask::GenerateMapFromPredicitontFloats(const TArray<float> &buffer){
    polygonDataCache.GenerateMapFromPredicitontFloats(buffer);
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
    FMeshedPolygonColorAttributes &attributes
){
    polygonDataCache.ColoredHeatMap(
        image, attributes
    );
    taskCompleted = true;
}
