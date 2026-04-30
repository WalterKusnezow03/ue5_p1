#include "PredictionTask.h"



void PredictionTask::Setup(ActorTrajectoryTracker *trackedActor){
    trackedActorPtr = trackedActor;
}

bool PredictionTask::IsValid(){
    return trackedActorPtr != nullptr;
}

void PredictionTask::PrepareRequestBinary(TArray<uint8> &buffer){
    
    PrepareRequestMap(polygonDataCache);

    //WRITE BINARY
    //polygon.AppendFlagMap(buffer); //deprecated!
    polygonDataCache.AppendFlagMapAsFloat(buffer);
    polygonDataCache.AppendTimeMap(buffer);

    //WriteData(buffer);
}

void PredictionTask::PrepareRequestMap(FMeshedPolygonTrajectoryLayered &polygonData){
    if(trackedActorPtr){
        polygonData.ClearFlags(); //clear previous map


        FVector location = trackedActorPtr->ActorLocation();

        //copy location for real result generation
        locationOfRequest = location;

        float radius = 100.0f; //stays the same!

        //FMeshedPolygonTrajectoryLayered polygonData;
        polygonProxy.CollectPolygon(
            location,
            radius,
            polygonData
        );

        TArray<Trajectory> trajectories = trackedActorPtr->localTrajectories();
        polygonData.EmbedTrajectories(trajectories);
    }
}




//prepares result binary!
bool PredictionTask::TickVisiblityCheck(TArray<uint8> &resultbytes){
    if(IsValid()){
        if(trackedActorPtr->IsFlaggedVisible()){
            //task is finished now, prepare bin for NN python / shared mem
            resultbytes.Empty();

            ////----- !!!! ----- TODO ----- !!!! -----

            //embed player position into a other flag grid, same size as polygon data grid
            //add player pos single!
            //append to bytes
            FVector location = trackedActorPtr->ActorLocation();
            polygonDataCache.EmbedResultPosition(location);
            polygonDataCache.AppendResultMapAsFloat(resultbytes);

            return true;
        }
    }
    return false;
}
        



bool PredictionTask::IsSameActor(AActor *actorCheck){
    if(actorCheck){
        if(trackedActorPtr){
            return trackedActorPtr->GetActor() == actorCheck;
        }
    }
    return false;
}