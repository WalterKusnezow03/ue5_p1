#include "PredictionTaskBase.h"
#include "NNCommunicationPlugin/Request/FNNRequestPackage.h"

void PredictionTaskBase::Reset(){
    taskStarted = false;
    taskCompleted = true;

}

bool PredictionTaskBase::WaitingForGroundTruth(){
    return taskStarted;
}
bool PredictionTaskBase::TaskCompleted(){
    return !WaitingForGroundTruth() && taskCompleted;
}

bool PredictionTaskBase::IsValid(){
    //return trackedActorPtr != nullptr;
    return true;
}

int PredictionTaskBase::ResultDataSizeBytes(){
    return 0; //to be overriden!
}


void PredictionTaskBase::EmbedRequestData(FNNRequestPackage &queue){
    //to be overriden!
}


void PredictionTaskBase::GenerateAndNotifyResultData(FNNRequestPackage *queue){
    if(queue){

        //example imlementation from NNPathfinder Plugin
        /*FMeshedPolygonTrajectoryLayeredInterface &data = GetPolygonData();
        
        TArray<IPathfinderNNInterface *> interfaces = queue->GetSubscribedActors();
        for (int i = 0; i < interfaces.Num(); i++){
            if(IPathfinderNNInterface *current = interfaces[i]){
                bool useVisibility = false; //ignore visibility 
                //polygonDataCache.NotifyVisiblePositionsFor(current, useVisibility);

                data.NotifyVisiblePositionsFor(current, useVisibility);
            }
        }*/
    }
}






//start task and refill / reload the polygon data cache
//to be appended to request binary / model input

void PredictionTaskBase::UpdateTaskStartedFlags(){
    taskStarted = true;
    taskCompleted = false;
}



//onnx input for ONNX Model

void PredictionTaskBase::PrepareRequestBinary(FONNXModelInput &model){
    UpdateTaskStartedFlags();
    /*FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    StartTaskBeforePrepareBinary(polygonDataCache);
    
    //if appending binary data failed: reset task and clean buffer
    if(!polygonDataCache.PrepareAppendRequestBinary(model)){
        model.Empty();
        Reset(); //clear task.
        return;
    }*/
}

void PredictionTaskBase::PrepareRequestBinary(TArray<uint8> &buffer){
    UpdateTaskStartedFlags();
    /*FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    StartTaskBeforePrepareBinary(polygonDataCache);

    //if appending binary data failed: reset task and clean buffer
    if(!polygonDataCache.PrepareAppendRequestBinary(buffer)){
        buffer.Empty();
        Reset(); //clear task.
        return;
    }*/
}



//prepares result binary!
bool PredictionTaskBase::TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes){
    
    if(IsValid()){
        //DebugHelper::showScreenMessage("PredictionTask::TickTask Z", FColor::Blue);
        //if(taskStarted && !taskCompleted){ //new condition
        if(taskStarted){ //new condition
            
            
            /*//DebugHelper::showScreenMessage("PredictionTask::TickTask A", FColor::Blue);
            if(trackedActorPtr->IsFlaggedVisibleReset()){
                //task is finished now, prepare bin for NN python / shared mem
                resultbytes.Empty();
                //DebugHelper::showScreenMessage("PredictionTask::TickTask B", FColor::Blue);
                

                FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();

                //embed player position into a other flag grid, same size as polygon data grid
                //add player pos single!
                //append to bytes
                FVector location = trackedActorPtr->ActorLocation();
                polygonDataCache.EmbedResultPosition(location);
                polygonDataCache.AppendGroundTruth(resultbytes);

                //reset flag
                taskStarted = false;

                return true;
            }*/

            return true;
        }
    }
    return false;
}
      
//Heat map prediction embedding
void PredictionTaskBase::ProcessFromPredictionBytes(const TArray<uint8> &buffer){
    //FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    //polygonDataCache.ProcessFromPredictionBytes(buffer);
}

void PredictionTaskBase::ProcessFromPredictionFloats(const TArray<float> &buffer){
    //FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    //polygonDataCache.ProcessFromPredictionFloats(buffer);
}

void PredictionTaskBase::MarkTaskFinished(){
    taskCompleted = true;
}



