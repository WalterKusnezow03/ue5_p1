#include "NNRequestHandle.h"
#include "DebugPlugin/DebugHelper.h"


void NNRequestHandle::BeginPlay(){
    float intervall = 0.5f; //seconds
    int listTrajectoriesmaxSize = 20; //max trajectories
    actorTracker.Setup(intervall, listTrajectoriesmaxSize);
    task.Reset();
    batchTask.Load();
    alreadyReadPredictionData = false;
}

bool NNRequestHandle::TaskCompleted(){
    return task.TaskCompleted();
}



void NNRequestHandle::Tick(FNNRequestHandleTickData &tickData){
    LoadBatchIfNotDoneYet(tickData);
    actorTracker.Tick(tickData.deltatime);
    
    //tick in connected? samples can be collected without.
    TickTask(tickData);
    
    //process result / prediction data
    TickReadDataResult(tickData);


    //if a task will be completed: reload next task automatically.
    PredictNextTask(tickData);
}


void NNRequestHandle::LoadBatchIfNotDoneYet(FNNRequestHandleTickData &tickData){
 
    //if the binary wasnt loaded and learned yet: load
    if(tickData.bBatchBinaryDataNeeded && !batchTask.BatchPrepared(GetSampleType())){
        DebugHelper::logMessage("ANNPathFinderSocket::LoadBatchIfNotDoneYet");

        TArray<uint8> &buffer = tickData.batchDataOut;
        batchTask.PrepareBatchBinary(buffer, GetSampleType());
        if (buffer.Num() > 0) //size must be valid
        {
            tickData.bBatchBinaryOutChanged = true; //flag for writing to shared memory
            //WriteData(frameNameBatch, buffer);
        }
    }
}

EPolygonSampleType NNRequestHandle::GetSampleType(){
    return task.GetSampleType();
}
void NNRequestHandle::UpdateSampleType(EPolygonSampleType type){
    task.UpdateSampleType(type);
}

//flags a aactor as spotted
void NNRequestHandle::FlagVisible(AActor *actor){
    if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
        actorTracker.FlagVisible(actor);
    }
}


// ---- subrscribe observer pattern ----
void NNRequestHandle::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
    }
    if(interfaceNotify && actor){
        requests.Subscribe(interfaceNotify, actor);
    }
}


//call this once a task is completed
void NNRequestHandle::PredictNextTask(FNNRequestHandleTickData &tickData){
    if(requests.HasTasks()){
        
        /*//get next prediction actor
        if(AActor *front = requests.frontActor()){
            PredictNode(front); //if a task is still running right now, it will be discared.
            //the list stays the same.
        }*/
        if(FPathFinderNNRequestPackage *package = requests.frontPackage()){
            PredictNode(package, tickData);
        }
    }
}


bool NNRequestHandle::PredictNodeAllowed(){
    //one task at a time for now.
    if(task.IsValid()){
        if(!task.TaskCompleted()){
            return false;
        }
    }
    return true;
}

//prepares the task by:
//adding actor to tracking if needed
//embedding the enemy vision into the task from the package
bool NNRequestHandle::PrepareTaskFor(FPathFinderNNRequestPackage *package){
    if(!package){
        return false;
    }

    AActor *actor = package->GetActor();
    if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
        task.Setup(actorTracker.FindIfTracked(actor));
        if(task.IsValid()){
            //on task start: embed enemy postions for this actor.
            task.EmbedEnemyPositionsAndVision(*package);
            return true;
        }
    }
    return false;
}

void NNRequestHandle::UpdateRequestBinaryFor(FNNRequestHandleTickData &tickData){
    //choose correct input
    if(tickData.useOnnxInput){
        task.PrepareRequestBinary(tickData.modelInput);
    }else{
        TArray<uint8> &requestBinary = tickData.requestDataOut;
        requestBinary.Empty();
        task.PrepareRequestBinary(requestBinary);
    }
}

void NNRequestHandle::PredictNode(
    //AActor *actor
    FPathFinderNNRequestPackage *package,
    FNNRequestHandleTickData &tickData
){
    if(!PredictNodeAllowed()){
        return;
    }

    /*//DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION - TRY A");
    //one task at a time for now.
    if(task.IsValid()){
        if(!task.TaskCompleted()){
            return;
        }
    }*/

    if(PrepareTaskFor(package)){
        //choose correct input
        UpdateRequestBinaryFor(tickData);

        if(task.IsValid()){
            tickData.bRequestBinaryOutChanged = true;
            //write num bytes expected
            tickData.expectedResultBytes = task.ResultDataSizeBytes();
            alreadyReadPredictionData = false;

            //WriteDataRequest(requestBinary, task.ResultDataSizeBytes());
        }
        //if data invalid, task is resettet
        else{
            task.Reset();
        }



    }else{
        task.Reset();
    }





    // ---- IS REFACTURED ! -----
    // ---- DEPRECATED ----
    /*
    if(!package){
        return;
    }
    AActor *actor = package->GetActor();

    DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION - TRY B");

    if (actor){
        //add if needed (filled with self location with default size buffer)
        actorTracker.AddTrackedActorIfNeeded(actor);
        task.Setup(actorTracker.FindIfTracked(actor));
        if(task.IsValid()){
            //on task start: embed enemy postions for this actor.
            task.EmbedEnemyPositionsAndVision(*package);

            //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION");
            //DebugHelper::showScreenMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION", FColor::Red);

            //no onnx support yet
            //TArray<uint8> &requestBinary = tickData.requestDataOut;
            //task.PrepareRequestBinary(requestBinary);

            if(tickData.useOnnxInput){
                task.PrepareRequestBinary(tickData.modelInput);
            }else{
                TArray<uint8> &requestBinary = tickData.requestDataOut;
                task.PrepareRequestBinary(requestBinary);
            }

            
            
            
            if(task.IsValid()){
                tickData.bRequestBinaryOutChanged = true;
                //write num bytes expected
                tickData.expectedResultBytes = task.ResultDataSizeBytes();

                //WriteDataRequest(requestBinary, task.ResultDataSizeBytes());
            }
            //if data invalid, task is resettet
            else{
                task.Reset();
            }
        }
    }*/
}




void NNRequestHandle::TickTask(FNNRequestHandleTickData &tickData){
    if(task.IsValid() == false){
        //DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask NOT VALID", FColor::Red);
        return;
    }
    if(!task.WaitingForGroundTruth()){
        return;
    }

    //live update Widget Heatmap
    GenerateLiveImage();

    //DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask", FColor::Red);

    TArray<uint8> &groundTruthBinary = tickData.groundTruthBinaryOut;

    //check visibility of target and prepare binary.
    if(task.TickVisiblityCheckAndPrepareGroundTruthBinary(groundTruthBinary)){
        DebugHelper::logMessage("NNRequestHandle::TickTask FINISH GT"); 
        GenerateResultImage();
        //GenerateResultImageChannels();

        if(groundTruthBinary.Num() > 0){
            //flag ground truth data written.
            tickData.bGroundTruthBinaryOutChanged = true;
            batchTask.AddSample(task.GetPolygonData());
            DebugHelper::logMessage("NNRequestHandle::TickTask FINISH GT ADD SAMPLE");
            
        }
        //GenerateResultImage();
        task.MarkTaskFinished();
        task.Reset();
    }
    
    
    /*if(groundTruthBinary.Num() > 0){
        //flag ground truth data written.
        tickData.bGroundTruthBinaryOutChanged = true;

        DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask OK", FColor::Orange);

        //since data and ground truth are ready:
        //add to batch
        batchTask.AddSample(task.GetPolygonData());

        //WriteDataGroundTruth(groundTruthBinary);

        // --- NEW HERE ---
        //mark as finished
        DebugHelper::logMessage("NNRequestHandle::TickTask FINISH GT");
        GenerateResultImage();
        task.MarkTaskFinished();
        task.Reset();
    }*/
}







void NNRequestHandle::TickReadDataResult(FNNRequestHandleTickData &tickData){
    if(!TaskCompleted()){
        if(alreadyReadPredictionData){
            return;
        }

        if(tickData.bHasPredictionData){
            alreadyReadPredictionData = true;
            ReadDataResult(tickData.predictionData);
            FString message = "NNRequestHandle::REQUEST FINISH Arrived IMPLICIT!";
            DebugHelper::logMessage(message);
            //mark as finished
            //task.Reset();
        }
    }


    /*
    if(task.TaskCompleted() == false){
        //DebugHelper::showScreenMessage("ANNPathFinderSocket::Tick Wait for Result", FColor::Cyan);
        //--> prevent python message:
        //if marked ready:
            //copy result:
            //flag not ready
        if(frameManager.TryReadReadyFlag(frameNameResult)){
            ReadDataResult();
            frameManager.MarkReadyFalse(frameNameResult); //mark as read

            FString message = "ANNPathFinderSocket::REQUEST FINISH Arrived IMPLICIT!";
            DebugHelper::logMessage(message);

            //DebugHelper::showScreenMessage(message, FColor::Red);
            task.Reset();
        }
    }*/
}

//only call from ONNX NN PathFinder !
void NNRequestHandle::ReadDataResultImmidiate(TArray<float> &data){
    if(data.Num() > 0){

        FString message = "NNRequestHandle::REQUEST FINISH Arrived!";
        DebugHelper::logMessage(message);

        //generate
        task.ProcessFromPredictionFloats(data);
        GenerateAndNotifyResultPositionsForRequestQueue(); //notify registered actors to the task.
        //GenerateResultImage();
        GeneratePredictionImage();

        //IS WRONG HERE: ONCE GT IS REAL!
        //task.MarkTaskFinished();
        //mark as finished! (? should be correct)
        //task.Reset();
    }
}

void NNRequestHandle::ReadDataResult(TArray<uint8> &bufferPrediction){
    
    /// create heat map /paste result from prediction
    task.ProcessFromPredictionBytes(bufferPrediction);
    
    //notify
    GenerateAndNotifyResultPositionsForRequestQueue();
    GeneratePredictionImage();

    /*

    //generate result image (for storage and subscribed widget listeners)
    GenerateResultImage();
    GenerateResultImageChannels();
    task.MarkTaskFinished();*/

    //task.Reset();
}

//notfiy prediction positions
void NNRequestHandle::GenerateAndNotifyResultPositionsForRequestQueue(){
    //create result for queue ------> TODO!
    TArray<FVector> positions;
    task.GenerateAndNotifyResultPositions(requests.frontPackage());
    requests.PopFront();
}

//looks ok.
void NNRequestHandle::GeneratePredictionImage(){
    Image image;
    FMeshedPolygonColorAttributes attributes(
        FColor(0, 0, 255, 0),       // FColor colorMinHeatIn,
        FColor(255, 0, 0, 255),     // FColor colorMaxHeatIn,
        FColor(255, 255, 255, 255), // FColor colorPolygonFlaggedIn,
        FColor(0,0,0,0),            // FColor colorViewGridIn,
        FColor(FColor::Yellow),     // FColor colorTrjacetoryIn,
        FColor(0, 0, 0, 0)          // FColor playerPosResultIn
    );

    task.ColoredHeatMap(
        image, //Image &image,
        attributes
    );
    NotifyHeatMapReceivers(image);
}

void NNRequestHandle::GenerateLiveImage(){
    Image image;
    FVector pos;
    ColorizedWidgetImage(image, pos);
    heatMapReceivers.NotifyAll(image, pos);
}

void NNRequestHandle::ColorizedWidgetImage(Image &image, FVector &worldPosPivot){
    FMeshedPolygonColorAttributes attributes(
        FColor(0, 0, 0, 255),     // FColor colorMinHeatIn,
        FColor(255, 0, 0, 255),     // FColor colorMaxHeatIn,
        FColor(255, 255, 255, 255), // FColor colorPolygonFlaggedIn,
        FColor(0,0,0,0),       // FColor colorViewGridIn,
        FColor(0,0,0,0), //FColor(FColor::Yellow),     // FColor colorTrjacetoryIn,
        FColor(0, 240, 0, 255)      // FColor playerPosResultIn
    );
    task.ColoredHeatMapWithTemporaryPlayer(
        image, //Image &image,
        attributes
    );

    //of FMeshedPolygonTrajectoryLayeredInterface
    worldPosPivot = task.GetPolygonData().BottomLeft();
}

void NNRequestHandle::GenerateResultImage(){
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

    task.ColoredHeatMap(
        image, //Image &image,
        attributes
    );
    //NotifyHeatMapReceivers(image);
    AddHeatMapSampleToStorage(image);
}

void NNRequestHandle::AddHeatMapSampleToStorage(Image &image){
    if(!saveHeatMapsEnabled){
        return;
    }
    heatMaps.Add(image);
}

void NNRequestHandle::GenerateResultImageChannels(){
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
    task.ColoredLayersMap(heatMaps, attributes);
    
}


void NNRequestHandle::NotifyHeatMapReceivers(Image &image){
    //generate widget image
    //notfiy listeners
   
    FVector pos = task.GetPolygonData().BottomLeft();
    //ColorizedWidgetImage(image, pos);
    heatMapReceivers.NotifyAll(image, pos);
}

HeatMapReceivers &NNRequestHandle::GetHeatMapReceivers(){
    return heatMapReceivers;
}


void NNRequestHandle::EndPlay(){
    task.EndSave();
    SaveHeatMapsOnEndPlay();
    batchTask.EndSave();
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