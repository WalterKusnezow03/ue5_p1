#include "MovementPredictionTask.h"
#include "NNCommunicationPlugin/Request/FNNRequestPackage.h"


void MovementPredictionTask::Reset(){
    PredictionTaskBase::Reset();
    UnsubscribeFromActorTracker();
    trackedActorPtr = nullptr;
    sampleData.ResetSampleData();
}

void MovementPredictionTask::Setup(ActorTrajectoryTracker *trackedActor){
    UnsubscribeFromActorTracker();
    trackedActorPtr = trackedActor;
    SubscribeToActorTracker();
}

void MovementPredictionTask::UnsubscribeFromActorTracker(){
    if(trackedActorPtr){
        trackedActorPtr->RemoveSubscriber(this);
    }
}

void MovementPredictionTask::SubscribeToActorTracker(){
    if(trackedActorPtr){
        trackedActorPtr->AddSubscriber(this);
    }
}




bool MovementPredictionTask::IsValid(){
    return trackedActorPtr != nullptr;
}

SampleInterface *MovementPredictionTask::GetSampleData(){
    return &sampleData;
}


bool MovementPredictionTask::IsSameActor(AActor *actorCheck){
    if(actorCheck){
        if(trackedActorPtr){
            return trackedActorPtr->GetActor() == actorCheck;
        }
    }
    return false;
}





void MovementPredictionTask::EmbedRequestData(FNNRequestPackage &queue){
    // todo
    // will likely stay empty, no external data needed, just the tracked actor
}



void MovementPredictionTask::GenerateAndNotifyResultData(FNNRequestPackage *queue){
    if(queue){
       
        
        TArray<IBaseNNInterface *> interfaces = queue->GetSubscribedActors();
        for (int i = 0; i < interfaces.Num(); i++){
            /*if(IPathfinderNNInterface *current = Cast<IPathfinderNNInterface>(interfaces[i])){
                

                data.NotifyVisiblePositionsFor(current, useVisibility);
            }*/
        }
    }
}






void MovementPredictionTask::PrepareRequestBinary(FONNXModelInput &model){
    PredictionTaskBase::PrepareRequestBinary(model);

    PrepareRequestMap(sampleData);

    //if appending binary data failed: reset task and clean buffer
    if(!sampleData.PrepareAppendRequestBinary(model)){
        model.Empty();
        Reset(); //clear task.
        return;
    }
}

void MovementPredictionTask::PrepareRequestBinary(TArray<uint8> &buffer){
    PredictionTaskBase::PrepareRequestBinary(buffer);
    
    PrepareRequestMap(sampleData);
    
    //if appending binary data failed: reset task and clean buffer
    if(!sampleData.PrepareAppendRequestBinary(buffer)){
        buffer.Empty();
        Reset(); //clear task.
        return;
    }
}


//deprecated naming but collects all polygon data from the area
//requested, prepares the data to fit unet or other
//and embeds the player (tracked actor) trajectories
void MovementPredictionTask::PrepareRequestMap(FMovementSample &polygonData){
    if(trackedActorPtr){
        FVector location = trackedActorPtr->ActorLocation();
        locationOfRequest = location;
        
        //get data
        TArray<Trajectory> trajectories = trackedActorPtr->localTrajectories();

        //embed data
        sampleData.EmbedTrajectories(trajectories);
    }
}


void MovementPredictionTask::OnTrajectoryAddedWorldSpace(Trajectory other){
    //move to local space by location of request!
    //no rotation inverse used, just translation

    //AB = B - A
    other -= locationOfRequest;
    sampleData.AddGroundTruthValue(other);
}

//prepares result binary!
bool MovementPredictionTask::TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes){
    
    if(PredictionTaskBase::TickVisiblityCheckAndPrepareGroundTruthBinary(resultbytes)){
        
        //check whether at least one new trajectory appeard
        //maybe a count (for example 5)
        
        //could be stored in a secondary buffer
        //which would be copied later into the original one

        //ein read mechanismus könnte auch helfen
        //und wäre einfacher zu implementieren
        if(sampleData.GroundTruthReady()){
            sampleData.AppendGroundTruth(resultbytes);
            return true;
        }

        //deprecated
        /*if(trackedActorPtr->IsFlaggedVisibleReset()){
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

            return true;
        }*/
    }
    return false;
}
      





