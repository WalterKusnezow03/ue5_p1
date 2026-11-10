#include "NNCommunicationSocket.h"

#include "DebugPlugin/DebugHelper.h"



//new
void ANNCommunicationSocket::OnBeginPlaySetupHandleStructure(){
    requestHandle = nullptr;
    requestTickData = nullptr;
}

void ANNCommunicationSocket::OnEndPlayCleanUpHandleStructure(){
    if (requestHandle){
        requestHandle->EndPlay();
    }

    delete requestHandle;
    requestHandle = nullptr;

    delete requestTickData;
    requestTickData = nullptr;
}

void ANNCommunicationSocket::OnBeginPlaySetupFrameNames(){
    //just as used with python process!
    //ANNPathFinderSF means ANNPathFinderSharedFrame
    frameNameRequest = "ANNPathFinderSFIN";
    frameNameResult = "ANNPathFinderSFRES";
    frameNameGroundThruth = "ANNPathFinderSFGT";
    frameNameBatch = "ANNPathFinderSFB";
    frameNameNNType = "ANNPathFinderSFNN";
}

void ANNCommunicationSocket::OnBeginPlayLaunchPython(){
    LaunchPythonProcess(
        "PathFinderNNExtension", 
        "nnpathfinder_server.py",
        "NNCommunicationPlugin",
        "venv/bin/python"
    );
}

void ANNCommunicationSocket::RequestHandleBeginPlay(){
    if(requestHandle){
        requestHandle->BeginPlay();
    }
}


void ANNCommunicationSocket::BeginPlay(){
    Super::BeginPlay();

    OnBeginPlaySetupHandleStructure();
    RequestHandleBeginPlay();
    OnBeginPlaySetupFrameNames();

    DebugHelper::logMessage("ANNCommunicationSocket BeginPlay - python");
    OnBeginPlayLaunchPython();
    sendPythonHeartBeat = true;
}

bool ANNCommunicationSocket::NNIsConnected(){
    return IsConnected();
}

void ANNCommunicationSocket::ResetRequestData(float deltatime){
    //prepare and execute task
    if(requestTickData){
        requestTickData->Reset();
        requestTickData->bBatchBinaryDataNeeded = trainingStartAllowed;
        requestTickData->deltatime = deltatime;
    }   
}

void ANNCommunicationSocket::TryReadPredictionData(){
    //result data arrived
    //TickReadDataResult(); to:
    if(!requestTickData){
        FString message = "ANNCommunicationSocket::REQUEST FINISH TICK DATA NULL!";
        DebugHelper::logMessage(message);
        return;
    }
    if(frameManager.TryReadReadyFlag(frameNameResult)){
        frameManager.TryReadDataTo(frameNameResult, requestTickData->predictionData);
        requestTickData->bHasPredictionData = true; //new prediction data was read.
        frameManager.MarkReadyFalse(frameNameResult); //mark as read
        FString message = "ANNCommunicationSocket::REQUEST FINISH Arrived IMPLICIT!";
        DebugHelper::logMessage(message);
    }else{
        //FString message = "ANNCommunicationSocket::Result Page Not Ready!";
        //DebugHelper::logMessage(message);
    }
}

void ANNCommunicationSocket::TickRequestHandle(){
    if(requestTickData && requestHandle){
        requestHandle->Tick(*requestTickData);
    }
}

void ANNCommunicationSocket::ProcessTrainingFlag(){
    if(!trainingStartAllowed || !requestTickData){
        return;
    }

    if(requestTickData->bBatchBinaryOutChanged){
        WriteData(frameNameBatch, requestTickData->batchDataOut);
        DebugHelper::logMessage("WRITE TRAIN BATCH ", requestTickData->batchDataOut.Num());
        trainingStartAllowed = false;
    }
}

void ANNCommunicationSocket::Tick(float deltatime){
    Super::Tick(deltatime);

    //prepare and execute task
    ResetRequestData(deltatime);

    //prepare result data if arrived (Prediction arrived)
    //result data arrived
    TryReadPredictionData();

    //tick request handler
    TickRequestHandle();

    ProcessTrainingFlag();

    if(requestTickData && requestTickData->bGroundTruthBinaryOutChanged){
        WriteDataGroundTruth(requestTickData->groundTruthBinaryOut);
    }

    //write request data
    if(requestTickData){
        if(requestTickData->bRequestBinaryOutChanged){
            DebugHelper::logMessage("WRITE REQUEST ", requestTickData->requestDataOut.Num());
            WriteDataRequest(
                requestTickData->requestDataOut,
                requestTickData->expectedResultBytes
            );
        }
    }
    

    
}

void ANNCommunicationSocket::SetTrainingAllowed(){
    trainingStartAllowed = true;
}



void ANNCommunicationSocket::TickSocketConnected(float deltatime){
    Super::TickSocketConnected(deltatime);
    // --- nothing needed here for now ---

    if(logConnection){
        DebugHelper::showScreenMessage("ANNPathFinderSocket::TickSocketConnected", FColor::Cyan);
    }

    
}





void ANNCommunicationSocket::EndPlay(const EEndPlayReason::Type EndPlayReason){
    OnEndPlayCleanUpHandleStructure();
    Super::EndPlay(EndPlayReason);
}








void ANNCommunicationSocket::WriteDataRequest(TArray<uint8> &buffer, int resultBytes){
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

void ANNCommunicationSocket::WriteDataGroundTruth(
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
void ANNCommunicationSocket::OnReceivePythonPrint(FString Output){
    Super::OnReceivePythonPrint(Output);
    
    // --- TODO! ---
    // Close the training frame when the Python process reports completion.
    if(CloseTrainSharedMemoryFrame(Output)){
        ClosePage(frameNameBatch);
    }


    if(logConnection){
        DebugHelper::logMessage("ANNPathFinderSocket::PYTHON ", Output);
    }
}


bool ANNCommunicationSocket::CloseTrainSharedMemoryFrame(const FString &message){
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
