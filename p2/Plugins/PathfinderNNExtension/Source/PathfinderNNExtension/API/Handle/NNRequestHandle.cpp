#include "NNRequestHandle.h"
#include "DebugPlugin/DebugHelper.h"


void NNRequestHandle::BeginPlay(){
    NNRequestHandleBase::BeginPlay();



    float intervall = 0.5f;           // seconds
    int listTrajectoriesmaxSize = 20; //max trajectories
    actorTracker.Setup(intervall, listTrajectoriesmaxSize);
}

void NNRequestHandle::OnBeginPlaySetupTask(){
    delete task;
    task = nullptr;
    task = new PredictionTask();
    task->Reset();
}

void NNRequestHandle::OnBeginPlaySetupBatchTask(){
    delete batchTask;
    batchTask = nullptr;

    unetSamples.Load();
    raymodelSamples.Load();
}

PredictionTask* NNRequestHandle::GetTask(){
    if(task){
        return static_cast<PredictionTask *>(task);
    }
    return nullptr;
}

void NNRequestHandle::Tick(FNNRequestHandleTickDataBase &tickData){
    actorTracker.Tick(tickData.deltatime);
    NNRequestHandleBase::Tick(tickData);

}

void NNRequestHandle::OnTick(){
    //actorTracker.Tick(tickData.deltatime);
}

FNNSampleSetBase *NNRequestHandle::GetBatchTask(){
    //replace with different ptrs
    //for different sample types!
    
    //With GetSampleType()

    EPolygonSampleType typeWanted = GetSampleType();
    if(unetSamples.IsSampleTypeOf(typeWanted)){
        return &unetSamples;
    }
    if(raymodelSamples.IsSampleTypeOf(typeWanted)){
        return &raymodelSamples;
    }
    
    //return batchTask;
    return nullptr;
}

EPolygonSampleType NNRequestHandle::GetSampleType(){
    if(PredictionTask* taskPtr = GetTask()){
        return taskPtr->GetSampleType();
    }

    return EPolygonSampleType::none;
}
void NNRequestHandle::UpdateSampleType(EPolygonSampleType type){
    if(PredictionTask* taskPtr = GetTask()){
        taskPtr->UpdateSampleType(type);
    }
}

//flags a aactor as spotted
void NNRequestHandle::FlagVisible(AActor *actor){
    if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
        actorTracker.FlagVisible(actor);
    }
}


// ---- subrscribe observer pattern ----

void NNRequestHandle::PredictData(
    IBaseNNInterface *interfaceNotify,
    FString nameTaskTarget,
    UObject *targetObject
){
    if(targetObject){
        if(AActor *casted = Cast<AActor>(targetObject)){
            actorTracker.AddTrackedActorIfNeeded(casted);
        }
    }
    NNRequestHandleBase::PredictData(
        interfaceNotify, nameTaskTarget, targetObject
    );

}



//prepares the task by:
//adding actor to tracking if needed
//embedding the enemy vision into the task from the package
bool NNRequestHandle::PrepareTaskFor(FNNRequestPackage *package){
    if(!package){
        return false;
    }
    PredictionTask *taskPtr = GetTask();
    if (!taskPtr){
        return false;
    }

    AActor *actor = Cast<AActor>(package->GetTargetObject());
    if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
        taskPtr->Setup(actorTracker.FindIfTracked(actor));
        if(taskPtr->IsValid()){
            //on task start: embed enemy postions for this actor.
            taskPtr->EmbedRequestData(*package);
            return true;
        }
    }
    return false;
}


void NNRequestHandle::OnTickTaskWaitForGroundTruth(){
    DebugHelper::logMessage("NNRequestHandle::OnTickTaskWaitForGroundTruth - GenerateLiveImage");
    GenerateLiveImage();
}

void NNRequestHandle::OnTickTaskArrivedGroundTruth(){
    DebugHelper::logMessage("NNRequestHandle::OnTickTaskArrivedGroundTruth - GenerateResultImage");
    GenerateResultImage();
}



void NNRequestHandle::OnPredictionArrive(){
    DebugHelper::logMessage("NNRequestHandle::OnTickTaskArrivedGroundTruth - GeneratePredictionImage");
    GeneratePredictionImage();
}





//looks ok.
void NNRequestHandle::GeneratePredictionImage(){
    if(PredictionTask* taskPtr = GetTask()){
        Image image;
        FMeshedPolygonColorAttributes attributes(
            FColor(0, 0, 255, 0),       // FColor colorMinHeatIn,
            FColor(255, 0, 0, 255),     // FColor colorMaxHeatIn,
            FColor(255, 255, 255, 255), // FColor colorPolygonFlaggedIn,
            FColor(0,0,0,0),            // FColor colorViewGridIn,
            FColor(FColor::Yellow),     // FColor colorTrjacetoryIn,
            FColor(0, 0, 0, 0)          // FColor playerPosResultIn
        );

        taskPtr->ColoredHeatMap(
            image, //Image &image,
            attributes
        );
        NotifyHeatMapReceivers(image);
    }
    
}

void NNRequestHandle::GenerateLiveImage(){
    Image image;
    FVector pos;
    ColorizedWidgetImage(image, pos);
    heatMapReceivers.NotifyAll(image, pos);
}

void NNRequestHandle::ColorizedWidgetImage(Image &image, FVector &worldPosPivot){
    if(PredictionTask* taskPtr = GetTask()){
        FMeshedPolygonColorAttributes attributes(
            FColor(0, 0, 0, 255),     // FColor colorMinHeatIn,
            FColor(255, 0, 0, 255),     // FColor colorMaxHeatIn,
            FColor(255, 255, 255, 255), // FColor colorPolygonFlaggedIn,
            FColor(0,0,0,0),       // FColor colorViewGridIn,
            FColor(0,0,0,0), //FColor(FColor::Yellow),     // FColor colorTrjacetoryIn,
            FColor(0, 240, 0, 255)      // FColor playerPosResultIn
        );
        taskPtr->ColoredHeatMapWithTemporaryPlayer(
            image, //Image &image,
            attributes
        );

        //of FMeshedPolygonTrajectoryLayeredInterface
        worldPosPivot = taskPtr->GetPolygonData().BottomLeft();
    }
}

void NNRequestHandle::GenerateResultImage(){
    if(PredictionTask* taskPtr = GetTask()){
        Image image;
        FMeshedPolygonColorAttributes attributes(
            FColor(0, 0, 255, 255),     // FColor colorMinHeatIn,
            FColor(255, 0, 0, 255),     // FColor colorMaxHeatIn,
            FColor(255, 255, 255, 255), // FColor colorPolygonFlaggedIn,
            FColor(FColor::Cyan),       // FColor colorViewGridIn,
            FColor(FColor::Yellow),     // FColor colorTrjacetoryIn,
            FColor(0, 255, 0, 255)      // FColor playerPosResultIn
        );

        /*for(int i = 0; i < 10; i++){
            DebugHelper::showScreenMessage("NNRequestHandle::GenerateResultImage!", FColor::Orange);
        }*/

        taskPtr->ColoredHeatMap(
            image, //Image &image,
            attributes
        );
        //NotifyHeatMapReceivers(image);
        AddHeatMapSampleToStorage(image);
    }
}

void NNRequestHandle::AddHeatMapSampleToStorage(Image &image){
    if(!saveHeatMapsEnabled){
        return;
    }
    heatMaps.Add(image);
}

void NNRequestHandle::GenerateResultImageChannels(){
    if(PredictionTask* taskPtr = GetTask()){
    
        if(!saveHeatMapsEnabled){
            return;
        }
        DebugHelper::logMessage("NNRequestHandle::GenerateResultImageChannels");
        FMeshedPolygonColorAttributes attributes(
            FColor(0, 0, 255, 255),     // FColor colorMinHeatIn,
            FColor(255, 0, 0, 255),     // FColor colorMaxHeatIn,
            FColor(255, 255, 255, 255), // FColor colorPolygonFlaggedIn,
            FColor(FColor::Cyan),       // FColor colorViewGridIn,
            FColor(FColor::Yellow),     // FColor colorTrjacetoryIn,
            FColor(0, 255, 0, 255)      // FColor playerPosResultIn
        );
        taskPtr->ColoredLayersMap(heatMaps, attributes);
    }
}


void NNRequestHandle::NotifyHeatMapReceivers(Image &image){
    //generate widget image
    //notfiy listeners
    DebugHelper::logMessage("NNRequestHandle::NotifyHeatMapReceivers Try");
    if(PredictionTask* taskPtr = GetTask()){
   
        FVector pos = taskPtr->GetPolygonData().BottomLeft();
        //ColorizedWidgetImage(image, pos);
        DebugHelper::logMessage("NNRequestHandle::NotifyHeatMapReceivers Notify");
        heatMapReceivers.NotifyAll(image, pos);
    }
}

HeatMapReceivers &NNRequestHandle::GetHeatMapReceivers(){
    return heatMapReceivers;
}


void NNRequestHandle::EndPlay(){
    NNRequestHandleBase::EndPlay();

    //task.EndSave();
    SaveHeatMapsOnEndPlay();
    
}

void NNRequestHandle::OnEndPlayBatchTaskSave(){
    unetSamples.EndSave();
    raymodelSamples.EndSave();
}

#include "StoragePlugin/Storage/ImageData/ImageWriter/ImageWriter.h"
void NNRequestHandle::SaveHeatMapsOnEndPlay(){
    
    if(saveHeatMapsEnabled){
        DebugHelper::logMessage("ANNPathFinderSocket::Save HeatMaps ", heatMaps.Num());
        ImageWriter::SaveImagesAsPngFromName(
            heatMaps,
            "NNPathFinderHeatMaps",
            "heatMap"
        );
    }
    
    heatMaps.Empty();
}


void NNRequestHandle::EnableHeatMapSaveOnEnd(bool flag){
    saveHeatMapsEnabled = flag;
}

bool NNRequestHandle::HeatMapSaveOnEndEnabled(){
    return saveHeatMapsEnabled;
}