#include "NNPathFinderSocket.h"

#include "DebugPlugin/DebugHelper.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"


void ANNPathFinderSocket::MakePathFinderSocketInstance(UWorld* World)
{
    if(PathFinderNNinstance()){
        return;
    }
    if (World){
        FActorSpawnParameters SpawnParams;
        SpawnParams.Name = TEXT("ANNPathFinderSocketActor");
        FVector Location = FVector::ZeroVector;
        FRotator Rotation = FRotator::ZeroRotator;
        ANNPathFinderSocket* Spawned = World->SpawnActor<ANNPathFinderSocket>(
            ANNPathFinderSocket::StaticClass(), Location, Rotation, SpawnParams
        );   
        instancePtr = Spawned;
    }
}


#include "PathfinderNNExtension/Settings/NNSetting.h"

void ANNPathFinderSocket::BeginPlay(){
    Super::BeginPlay();
    requestHandle.BeginPlay();

    DebugHelper::logMessage("ANNPathFinderSocket BeginPlay - python");

    frameNameRequest = "ANNPathFinderSFIN";
    frameNameResult = "ANNPathFinderSFRES";
    frameNameGroundThruth = "ANNPathFinderSFGT";
    frameNameBatch = "ANNPathFinderSFB";

    //Plugins/NNCommunicationPlugin/Source/NNCommunicationPlugin/Python/venv/bin/python
    LaunchPythonProcess(
        "PathFinderNNExtension", 
        "nnpathfinder_server.py",
        "NNCommunicationPlugin", //hier ist python installiert.
        "venv/bin/python"
    ); // finds working dir automatically


    //debug
    NNPluginSettings::LogSetting();
}

bool ANNPathFinderSocket::NNIsConnected(){
    return IsConnected();
}

void ANNPathFinderSocket::Tick(float deltatime){
    Super::Tick(deltatime);

    //prepare and execute task
    requestTickData.Reset();
    requestTickData.bBatchBinaryDataNeeded = trainingStartAllowed;
    requestTickData.deltatime = deltatime;

    //prepare result data if arrived (Prediction arrived)

    //result data arrived
    //TickReadDataResult(); to:
    if(frameManager.TryReadReadyFlag(frameNameResult)){
        frameManager.TryReadDataTo(frameNameResult, requestTickData.predictionData);
        requestTickData.bHasPredictionData = true; //new prediction data was read.
        frameManager.MarkReadyFalse(frameNameResult); //mark as read
        FString message = "ANNPathFinderSocket::REQUEST FINISH Arrived IMPLICIT!";
        DebugHelper::logMessage(message);
    }


    //tick request handler
    requestHandle.Tick(requestTickData);

    //LoadBatchIfNotDoneYet(); //to:
    //Load Batch if not done yet / training 
    if(trainingStartAllowed){
        if(requestTickData.bBatchBinaryOutChanged){
            WriteData(frameNameBatch, requestTickData.batchDataOut);
            DebugHelper::logMessage("WRITE TRAIN BATCH ", requestTickData.batchDataOut.Num());
        }
    }

    //todo: once training is finished: close the Train Batch Shared Memory page
    //must be also removed from python!




    //write ground truth binary if exists
    //TickTask(); to:
    if(requestTickData.bGroundTruthBinaryOutChanged){
        WriteDataGroundTruth(requestTickData.groundTruthBinaryOut);
        DebugHelper::logMessage("WRITE GROUND TRUTH ", requestTickData.groundTruthBinaryOut.Num());
    }

    if(requestTickData.bRequestBinaryOutChanged){
        DebugHelper::logMessage("WRITE REQUEST ", requestTickData.requestDataOut.Num());
        WriteDataRequest(
            requestTickData.requestDataOut,
            requestTickData.expectedResultBytes
        );
    }

    
    

    
   

    //LoadBatchIfNotDoneYet();
    //actorTracker.Tick(deltatime);
    
    //tick in connected? samples can be collected without.
    //TickTask();
    
    
    //TickReadDataResult();
    //DebugHelper::showScreenMessage("ANNPathFinderSocket::Tick", FColor::Cyan);


    //if a task will be completed: reload next task automatically.
    //PredictNextTask();
}


void ANNPathFinderSocket::SetTrainingAllowed(){
    trainingStartAllowed = true;
}


void ANNPathFinderSocket::EnableHeatMapSaveOnEnd(bool saveOnEnd){
    requestHandle.EnableHeatMapSaveOnEnd(saveOnEnd);
}


bool ANNPathFinderSocket::HeatMapSaveOnEndEnabled(){
    return requestHandle.HeatMapSaveOnEndEnabled();
}



void ANNPathFinderSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---
    //TickTask();

    if(logConnection){
        DebugHelper::showScreenMessage("ANNPathFinderSocket::TickSocketConnected", FColor::Cyan);
    }

    
}

void ANNPathFinderSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    requestHandle.EndPlay();
    Super::EndPlay(EndPlayReason);
}








//// ---- interaction ----


//flags a aactor as spotted
void ANNPathFinderSocket::FlagVisible(AActor *actor){
    requestHandle.FlagVisible(actor);

    /*if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
        actorTracker.FlagVisible(actor);
    }*/
}


// ---- subrscribe observer pattern ----
void ANNPathFinderSocket::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    requestHandle.PredictNode(interfaceNotify, actor);

    /*if(actor){
        actorTracker.AddTrackedActorIfNeeded(actor);
    }
    if(interfaceNotify && actor){
        requests.Subscribe(interfaceNotify, actor);
    }*/
}







void ANNPathFinderSocket::WriteDataRequest(TArray<uint8> &buffer, int resultBytes){
    FString message = FString::Printf(
        TEXT("ANNPathFinderSocket::WriteDataRequest %d, Result Bytes Expected %d"),
        buffer.Num(),
        resultBytes
    );
    DebugHelper::logMessage(message);
    //DebugHelper::showScreenMessage(message, FColor::Red);
    if(buffer.Num() > 0 && resultBytes > 0){
        
        // ---- result must exists to ask the NN for a result! (is not created in python!) ----
        frameManager.MakeSureExists(frameNameResult, resultBytes);
        WriteData(frameNameRequest, buffer);
    }
    //reseted inside request handle.
    /*else{
        task.Reset();
    }*/
}

void ANNPathFinderSocket::WriteDataGroundTruth(
    TArray<uint8> &buffer
){
    if(buffer.Num() > 0){
        FString message = FString::Printf(TEXT("ANNPathFinderSocket::WriteDataGroundTruth %d"),buffer.Num());
        //DebugHelper::showScreenMessage(message, FColor::Red);
        DebugHelper::logMessage(message);
        WriteData(frameNameGroundThruth, buffer);
    }
    
}

/// receives all incoming python prints
void ANNPathFinderSocket::OnReceivePythonPrint(FString Output){
    Super::OnReceivePythonPrint(Output);
    
    // --- TODO! ---
    //if finished: close train batch shared memory!
    //printed in python: "NNServerPathfinder_NetB_RUN_NN_BATCH_TRAIN_FINISHED"
    if(CloseTrainSharedMemoryFrame(Output)){
        //close shared memory from shared memory manager
        //notify shared page is closed!   
        ClosePage(frameNameBatch);
    }



    if(false){
        //DebugHelper::showScreenMessage("ANNPathFinderSocket::ReceivePythonPrint ", Output, FColor::Red);
        DebugHelper::logMessage("ANNPathFinderSocket::PYTHON ", Output);
        return;
    }
}


bool ANNPathFinderSocket::CloseTrainSharedMemoryFrame(const FString &message){
    //if finished: close train batch shared memory!
    //printed in python: "NNServerPathfinder_NetB_RUN_NN_BATCH_TRAIN_FINISHED"
    if(message.Contains("RUN_NN_BATCH_TRAIN_FINISHED")){
        return true;
    }
    //nicht zwingend net B !
    if(message.Contains("NNServerPathfinder_NetB_RUN_NN_BATCH_TRAIN_FINISHED")){
        return true;
    }
    return false;
}
