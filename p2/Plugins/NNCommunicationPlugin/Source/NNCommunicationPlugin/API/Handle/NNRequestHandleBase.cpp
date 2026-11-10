#include "NNRequestHandleBase.h"
#include "DebugPlugin/DebugHelper.h"


void NNRequestHandleBase::BeginPlay(){
    OnBeginPlaySetupTask();
    OnBeginPlaySetupBatchTask();
    alreadyReadPredictionData = false;
}

void NNRequestHandleBase::OnBeginPlaySetupTask(){
    delete task;
    task = nullptr;
    task = new PredictionTaskBase();
    task->Reset();
}

void NNRequestHandleBase::OnBeginPlaySetupBatchTask(){
    delete batchTask;
    batchTask = nullptr;
}

bool NNRequestHandleBase::TaskCompleted(){
    return task->TaskCompleted();
}

FNNSampleSetBase *NNRequestHandleBase::GetBatchTask(){
    return batchTask;
}

void NNRequestHandleBase::Tick(FNNRequestHandleTickDataBase &tickData){
    LoadBatchIfNotDoneYet(tickData);

    OnTick();

    //tick in connected? samples can be collected without.
    TickTask(tickData);
    
    //process result / prediction data
    TickReadDataResult(tickData);


    //if a task will be completed: reload next task automatically.
    PredictNextTask(tickData);
}

void NNRequestHandleBase::LoadBatchIfNotDoneYet(FNNRequestHandleTickDataBase &tickData){
    // Batch loading is temporarily disabled until its implementation is restored.
    if(!tickData.bBatchBinaryDataNeeded){
        return;
    }
    if(FNNSampleSetBase *batchTaskPtr = GetBatchTask()){
        //batch was not prepared yet: load
        if(!batchTaskPtr->BatchPrepared()){
            DebugHelper::logMessage("NNRequestHandleBase::LoadBatchIfNotDoneYet");

            TArray<uint8> &buffer = tickData.batchDataOut;
            batchTaskPtr->PrepareBatchBinary(buffer);
            if (buffer.Num() > 0) //size must be valid
            {
                tickData.bBatchBinaryOutChanged = true; //flag for writing to shared memory
                //WriteData(frameNameBatch, buffer);
            }
        }
    }
}


// ---- subrscribe observer pattern ----
void NNRequestHandleBase::PredictData(
    IBaseNNInterface *interfaceNotify,
    FString nameTaskTarget,
    UObject *targetObject
){
    if(interfaceNotify){
        requests.Subscribe(interfaceNotify, nameTaskTarget, targetObject);
    }
}


//call this once a task is completed
void NNRequestHandleBase::PredictNextTask(FNNRequestHandleTickDataBase &tickData){
    if(requests.HasTasks()){
        
        if(FNNRequestPackage *package = requests.frontPackage()){
            PredictData(package, tickData);
        }
    }
}


bool NNRequestHandleBase::PredictDataAllowed(){
    //one task at a time for now.
    if(task && task->IsValid()){
        if(!task->TaskCompleted()){
            return false;
        }
    }
    return true;
}

//prepares the task by generating the nesecarry data for the NN Input (which will be copied afterwards)
bool NNRequestHandleBase::PrepareTaskFor(FNNRequestPackage *package){
    if(!package){
        return false;
    }

    // Actor-to-task binding is temporarily disabled until a concrete task is restored.
    return true;
}

void NNRequestHandleBase::UpdateRequestBinaryFor(FNNRequestHandleTickDataBase &tickData){
    if(!task){
        return;
    }

    //choose correct input
    if(tickData.useOnnxInput){
        task->PrepareRequestBinary(tickData.modelInput);
    }else{
        TArray<uint8> &requestBinary = tickData.requestDataOut;
        requestBinary.Empty();
        task->PrepareRequestBinary(requestBinary);
    }
}

void NNRequestHandleBase::PredictData(
    //AActor *actor
    FNNRequestPackage *package,
    FNNRequestHandleTickDataBase &tickData
){
    if(!PredictDataAllowed()){
        return;
    }
    if(!task){
        return;
    }

    if(PrepareTaskFor(package)){
        //prepare request binary data (while choosing correct input)
        UpdateRequestBinaryFor(tickData);

        if(task->IsValid()){
            tickData.bRequestBinaryOutChanged = true;
            //write num bytes expected for prediction
            tickData.expectedResultBytes = task->ResultDataSizeBytes();
            alreadyReadPredictionData = false;

            //WriteDataRequest(requestBinary, task.ResultDataSizeBytes());
        }
        //if data invalid, task is resettet
        else{
            task->Reset();
        }



    }else{
        task->Reset();
    }
}




void NNRequestHandleBase::TickTask(FNNRequestHandleTickDataBase &tickData){
    if(!task){
        return;
    }
    if(task->IsValid() == false){
        //DebugHelper::showScreenMessage("NNRequestHandleBase::TickTask NOT VALID", FColor::Red);
        return;
    }
    if(!task->WaitingForGroundTruth()){
        return;
    }

    //live update of any display data (such as widgets)
    OnTickTaskWaitForGroundTruth();

    //Update the GT Binary which may be used for nn live training
    TArray<uint8> &groundTruthBinary = tickData.groundTruthBinaryOut;

    //check visibility of target and prepare binary.
    if(task->TickVisiblityCheckAndPrepareGroundTruthBinary(groundTruthBinary)){
        DebugHelper::logMessage("NNRequestHandleBase::TickTask FINISH GT"); 
        
        //since the ground truth data is valid, we can process further actions
        //and added to the sample set.
        OnTickTaskArrivedGroundTruth();

        //flag GT Data as valid
        if(groundTruthBinary.Num() > 0){
            //flag ground truth data written.
            tickData.bGroundTruthBinaryOutChanged = true;
            
            DebugHelper::logMessage("NNRequestHandleBase::TickTask FINISH GT ADD SAMPLE");

            //save smaple before the task is reset
            OnTickTaskArrivedGroundTruthValid();
        }

        //reset the task, so the next task from the queue can be executed
        task->MarkTaskFinished();
        task->Reset();
    }
}





void NNRequestHandleBase::OnTickTaskWaitForGroundTruth(){
    //GenerateLiveImage for example
}

void NNRequestHandleBase::OnTickTaskArrivedGroundTruth(){
    //Generate an image aswell 
}

void NNRequestHandleBase::OnTickTaskArrivedGroundTruthValid(){
    //since the ground truth is valid it can be added as sample for training
    DebugHelper::logMessage("NNRequestHandleBase::OnTickTaskArrivedGroundTruthValid - Append Sample to Batch");
    FNNSampleSetBase *batchTaskPtr = GetBatchTask();
    if(task && batchTaskPtr){
        if(SampleInterface *sample = task->GetSampleData()){
            batchTaskPtr->AddSample(*sample);
        }
    }
}



void NNRequestHandleBase::OnPredictionArrive(){
    //prediction from nn might be displayed in a widget for example.
}



void NNRequestHandleBase::TickReadDataResult(FNNRequestHandleTickDataBase &tickData){
    //if the task is not completed yet, the prediction data can be processed once
    if(!TaskCompleted()){
        if(alreadyReadPredictionData){
            return;
        }
        //process the nn prediction data, but only to so once
        if(tickData.bHasPredictionData){
            alreadyReadPredictionData = true;
            ReadDataResult(tickData.predictionData);
            FString message = "NNRequestHandleBase::REQUEST FINISH Arrived IMPLICIT!";
            DebugHelper::logMessage(message);
            
        }
    }
}

//only call from ONNX NN PathFinder !
void NNRequestHandleBase::ReadDataResultImmidiate(TArray<float> &data){
    //process prediction data immidiate
    if(!task){
        return;
    }

    if(data.Num() > 0){

        FString message = "NNRequestHandleBase::REQUEST FINISH Arrived!";
        DebugHelper::logMessage(message);

        //generate result data 
        task->ProcessFromPredictionFloats(data);
        
        //notify registered actors to the task.
        GenerateAndNotifyResultPositionsForRequestQueue();

        //GeneratePredictionImage();
        OnPredictionArrive();

    }
}

void NNRequestHandleBase::ReadDataResult(TArray<uint8> &bufferPrediction){
    if(!task){
        return;
    }

    // (for example create heat map) / paste result from prediction
    task->ProcessFromPredictionBytes(bufferPrediction);
    
    //notify
    GenerateAndNotifyResultPositionsForRequestQueue();

    //use processed prediction data to display in a widget for example
    OnPredictionArrive();

    //reminder:
    //is not executed yet, only once Ground Truth is there!
    //Not executed: task.MarkTaskFinished();

}

//notfiy prediction positions
void NNRequestHandleBase::GenerateAndNotifyResultPositionsForRequestQueue(){
    if(task){
        task->GenerateAndNotifyResultData(requests.frontPackage());
    }
    
    
    
    // Result generation is temporarily disabled until a concrete prediction task exists.
    
    //pop the finished front task
    requests.PopFront();
}




void NNRequestHandleBase::EndPlay(){
    if(task){
        task->EndSave();
        delete task;
        task = nullptr;
    }
    OnEndPlayBatchTaskSave();

    //SaveHeatMapsOnEndPlay();
    //batchTask.EndSave();
}

void NNRequestHandleBase::OnEndPlayBatchTaskSave(){
    if(FNNSampleSetBase *batchTaskPtr = GetBatchTask()){
        batchTaskPtr->EndSave();
    }
    if(batchTask){
        delete batchTask;
        batchTask = nullptr;
    }
}


