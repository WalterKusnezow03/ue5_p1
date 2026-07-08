#include "NNRequestHandle.h"
#include "DebugPlugin/DebugHelper.h"


void NNRequestHandle::BeginPlay(){
    float intervall = 0.5f; //seconds
    int listTrajectoriesmaxSize = 20; //max trajectories
    actorTracker.Setup(intervall, listTrajectoriesmaxSize);
    task.Reset();
    batchTask.Load();
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
 
    if(tickData.bBatchBinaryDataNeeded && !batchTask.BatchPrepared()){
        DebugHelper::logMessage("ANNPathFinderSocket::LoadBatchIfNotDoneYet");

        TArray<uint8> &buffer = tickData.batchDataOut;
        batchTask.PrepareBinary(buffer);
        if (buffer.Num() > 0) //size must be valid
        {
            tickData.bBatchBinaryOutChanged = true;

            //WriteData(frameNameBatch, buffer);
        }
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


void NNRequestHandle::PredictNode(
    //AActor *actor
    FPathFinderNNRequestPackage *package,
    FNNRequestHandleTickData &tickData
){
    


    //DebugHelper::logMessage("ANNPathFinderSocket::REQUEST PREDICT NEW POSITION - TRY A");
    //one task at a time for now.
    if(task.IsValid()){
        if(!task.TaskCompleted()){
            return;
        }
    }




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
            TArray<uint8> &requestBinary = tickData.requestDataOut;
            task.PrepareRequestBinary(requestBinary);
            
            if(task.IsValid()){
                tickData.bRequestBinaryOutChanged = true;
                //write num bytes expected
                tickData.expectedResultBytes = task.ResultGridSizeBytes();

                //WriteDataRequest(requestBinary, task.ResultGridSizeBytes());
            }
            //if data invalid, task is resettet
            else{
                task.Reset();
            }
        }
    }
}




void NNRequestHandle::TickTask(FNNRequestHandleTickData &tickData){
    if(task.IsValid() == false){
        //DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask NOT VALID", FColor::Red);
        return;
    }
    if(!task.WaitingForGroundTruth()){
        return;
    }

    //DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask", FColor::Red);

    TArray<uint8> &groundTruthBinary = tickData.groundTruthBinaryOut;

    task.TickVisiblityCheckAndPrepareGroundTruthBinary(groundTruthBinary);
    if(groundTruthBinary.Num() > 0){
        //flag ground truth data written.
        tickData.bGroundTruthBinaryOutChanged = true;

        DebugHelper::showScreenMessage("ANNPathFinderSocket::TickTask OK", FColor::Orange);

        //since data and ground truth are ready:
        //add to batch
        batchTask.AddSample(task.GetPolygonData());

        //WriteDataGroundTruth(groundTruthBinary);
    }
}







void NNRequestHandle::TickReadDataResult(FNNRequestHandleTickData &tickData){
    if(!TaskCompleted()){
        if(tickData.bHasPredictionData){
            ReadDataResult(tickData.predictionData);
            FString message = "NNRequestHandle::REQUEST FINISH Arrived IMPLICIT!";
            DebugHelper::logMessage(message);
            //mark as finished
            task.Reset();
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
        task.GenerateMapFromPredicitontFloats(data);
        GenerateAndNotifyResultPositionsForRequestQueue(); //notify registered actors to the task.
        GenerateResultImage();
        

        //mark as finished! (? should be correct)
        task.Reset();
    }
}

void NNRequestHandle::ReadDataResult(TArray<uint8> &bufferPrediction){
    
    /// create heat map from prediction
    task.GenerateMapFromPredicitontBytes(bufferPrediction);
    
    //notify
    GenerateAndNotifyResultPositionsForRequestQueue();

    //generate result image
    GenerateResultImage();

    //task.Reset();
}

void NNRequestHandle::GenerateAndNotifyResultPositionsForRequestQueue(){
    //create result for queue ------> TODO!
    TArray<FVector> positions;
    task.GenerateAndNotifyResultPositions(requests.frontPackage());
    requests.PopFront();
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

    task.ColoredHeatMap(
        image, //Image &image,
        attributes
    );
    
    heatMaps.Add(image);
}

    





void NNRequestHandle::EndPlay(){
    task.EndSave();
    SaveHeatMapsOnEndPlay();
    batchTask.EndSave();
}

#include "StoragePlugin/Storage/ImageData/ImageWriter/ImageWriter.h"
void NNRequestHandle::SaveHeatMapsOnEndPlay(){
    
    DebugHelper::logMessage("ANNPathFinderSocket::Save HeatMaps ", heatMaps.Num());
    ImageWriter::SaveImagesAsPngFromName(
        heatMaps,
        "NNPathFinderHeatMaps",
        "heatMap"
    );
    heatMaps.Empty();
}
