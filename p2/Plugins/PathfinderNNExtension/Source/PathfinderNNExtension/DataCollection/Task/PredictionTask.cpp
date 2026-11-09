#include "PredictionTask.h"
#include "NNCommunicationPlugin/Request/FNNRequestPackage.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"


void PredictionTask::Reset(){
    PredictionTaskBase::Reset();
    trackedActorPtr = nullptr;
    polygonDataUnetModel.Reset();
    polygonRayModel.Reset();

    //polygonDataCache.Reset();  //clear task polygon data
}

void PredictionTask::Setup(ActorTrajectoryTracker *trackedActor){
    trackedActorPtr = trackedActor;
}

bool PredictionTask::IsValid(){
    return trackedActorPtr != nullptr;
}

SampleInterface *PredictionTask::GetSampleData(){
   FMeshedPolygonTrajectoryLayeredInterface &data = GetPolygonData();
   return &data;
}

int PredictionTask::ResultDataSizeBytes(){
    FMeshedPolygonTrajectoryLayeredInterface &data = GetPolygonData();
    return data.ResultDataSizeBytes(); //prediction / result byte size of grid
}

void PredictionTask::EmbedEnemyPositionsAndVision(const TArray<FVisionCone*> &enemies){
    //polygonDataCache.EmbedEnemyVision(enemies);
    FMeshedPolygonTrajectoryLayeredInterface &data = GetPolygonData();
    data.EmbedEnemyVision(enemies);
}

void PredictionTask::EmbedRequestData(FNNRequestPackage &queue){
    TArray<FVisionCone*> positionsToEmbed;
    for(IBaseNNInterface *interface : queue.GetSubscribedActors()){
        if(IPathfinderNNInterface *pathfinderInterface = Cast<IPathfinderNNInterface>(interface)){
            positionsToEmbed.Add(&pathfinderInterface->GetVisionCone());
        }
    }
    EmbedEnemyPositionsAndVision(positionsToEmbed);
}



void PredictionTask::GenerateAndNotifyResultData(FNNRequestPackage *queue){
    if(queue){
        FMeshedPolygonTrajectoryLayeredInterface &data = GetPolygonData();
        
        TArray<IBaseNNInterface *> interfaces = queue->GetSubscribedActors();
        for (int i = 0; i < interfaces.Num(); i++){
            if(IPathfinderNNInterface *current = Cast<IPathfinderNNInterface>(interfaces[i])){
                bool useVisibility = false; //ignore visibility 
                //polygonDataCache.NotifyVisiblePositionsFor(current, useVisibility);

                data.NotifyVisiblePositionsFor(current, useVisibility);
            }
        }
    }
}






//onnx input for ONNX Model
void PredictionTask::UpdateSampleType(EPolygonSampleType type){
    sampleType = type;
}

EPolygonSampleType PredictionTask::GetSampleType(){
    return sampleType;
}

void PredictionTask::PrepareRequestBinary(FONNXModelInput &model){
    PredictionTaskBase::PrepareRequestBinary(model);

    FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    PrepareRequestMap(polygonDataCache);

    //if appending binary data failed: reset task and clean buffer
    if(!polygonDataCache.PrepareAppendRequestBinary(model)){
        model.Empty();
        Reset(); //clear task.
        return;
    }
}

void PredictionTask::PrepareRequestBinary(TArray<uint8> &buffer){
    PredictionTaskBase::PrepareRequestBinary(buffer);
    
    FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    PrepareRequestMap(polygonDataCache);
    //if appending binary data failed: reset task and clean buffer
    if(!polygonDataCache.PrepareAppendRequestBinary(buffer)){
        buffer.Empty();
        Reset(); //clear task.
        return;
    }
}


//deprecated naming but collects all polygon data from the area
//requested, prepares the data to fit unet or other
//and embeds the player (tracked actor) trajectories
void PredictionTask::PrepareRequestMap(FMeshedPolygonTrajectoryLayeredInterface &polygonData){
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
        polygonData.InvertFlagMap();

        //move trajectory times to space of -1..1 to be similar to binary occupancy map of polygons!
        TArray<Trajectory> trajectories = trackedActorPtr->worldTrajectoriesNormalizedTime(); 
        // trackedActorPtr->worldTrajectories();
        polygonData.EmbedTrajectories(trajectories);

        //polygonData.PrepareFitData();
    }
}


//prepares result binary!
bool PredictionTask::TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes){
    
    if(PredictionTaskBase::TickVisiblityCheckAndPrepareGroundTruthBinary(resultbytes)){
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

            return true;
        }
    }
    return false;
}
      
//Heat map prediction embedding
void PredictionTask::ProcessFromPredictionBytes(const TArray<uint8> &buffer){
    FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    polygonDataCache.ProcessFromPredictionBytes(buffer);
}

void PredictionTask::ProcessFromPredictionFloats(const TArray<float> &buffer){
    FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    polygonDataCache.ProcessFromPredictionFloats(buffer);
}

bool PredictionTask::IsSameActor(AActor *actorCheck){
    if(actorCheck){
        if(trackedActorPtr){
            return trackedActorPtr->GetActor() == actorCheck;
        }
    }
    return false;
}



void PredictionTask::ColoredHeatMapWithTemporaryPlayer(
    Image &image,
    FMeshedPolygonColorAttributes &attributes
){
    if(trackedActorPtr){
        FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
        FVector location = trackedActorPtr->ActorLocation();
        polygonDataCache.EmbedResultPositionTempoary(location);
    }
    ColoredHeatMap(image, attributes, true);
}

//for result output
void PredictionTask::ColoredHeatMap(
    Image &image,
    FMeshedPolygonColorAttributes &attributes,
    bool tmpPlayerPos
){
    FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    polygonDataCache.ColoredHeatMap(
        image, attributes, tmpPlayerPos
    );
}

void PredictionTask::ColoredLayersMap(
    TArray<Image> &images,
    FMeshedPolygonColorAttributes &attributes
){
    FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache = GetPolygonData();
    polygonDataCache.ColoredLayersMap(
        images, attributes
    );
}




//get by type
FMeshedPolygonTrajectoryLayeredInterface &PredictionTask::GetPolygonData(){
    return GetPolygonData(sampleType);
}

FMeshedPolygonTrajectoryLayeredInterface &PredictionTask::GetPolygonData(EPolygonSampleType type){
    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryLayered){
        return polygonDataUnetModel;
    }
    if(type == EPolygonSampleType::EMeshedPolygonTrajectoryRayModel){
        return polygonRayModel;
    }

    //fallback
    return polygonDataUnetModel;
    //return polygonDataCache;
}