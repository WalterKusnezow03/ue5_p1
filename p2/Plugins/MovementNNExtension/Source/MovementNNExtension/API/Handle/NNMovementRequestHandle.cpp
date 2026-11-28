#include "NNMovementRequestHandle.h"
#include "DebugPlugin/DebugHelper.h"


void NNMovementRequestHandle::BeginPlay(){
    NNRequestHandleBase::BeginPlay();

    float intervall = MillisToSeconds(100);

    int listTrajectoriesmaxSize = 40; //max trajectories
    actorTracker.Setup(intervall, listTrajectoriesmaxSize);
}



void NNMovementRequestHandle::OnBeginPlaySetupTask(){
    delete task;
    task = nullptr;
    task = new MovementPredictionTask();
    task->Reset();
}

void NNMovementRequestHandle::OnBeginPlaySetupBatchTask(){
    delete batchTask;
    batchTask = nullptr; //must be set to a proper object!

    if(loadSamples){
        //TODO!
        batchData.Load();
    }
}

FNNSampleSetBase *NNMovementRequestHandle::GetBatchTask(){
    return &batchData;
}

MovementPredictionTask *NNMovementRequestHandle::GetTask(){
    if(task){
        return static_cast<MovementPredictionTask *>(task);
    }
    return nullptr;
}


void NNMovementRequestHandle::OnTick(float deltatime){
    actorTracker.Tick(deltatime);
}


// ---- subrscribe observer pattern ----
void NNMovementRequestHandle::PredictData(
    IBaseNNInterface *interfaceNotify,
    FString nameTaskTarget,
    UObject *targetObject
){
    SubscribeActorIfNeeded(targetObject);
    NNRequestHandleBase::PredictData(interfaceNotify, nameTaskTarget, targetObject);
}


void NNMovementRequestHandle::SubscribeActorIfNeeded(
    UObject *targetObject
){
    if(targetObject){
        if(AActor *casted = Cast<AActor>(targetObject)){
            actorTracker.AddTrackedActorIfNeeded(casted);
        }
    }
}




void NNMovementRequestHandle::OnTickTaskWaitForGroundTruth(){
    //GenerateLiveImage for example
}

void NNMovementRequestHandle::OnTickTaskArrivedGroundTruth(){
    //Generate an image aswell 
}

void NNMovementRequestHandle::OnTickTaskArrivedGroundTruthValid(){
    NNRequestHandleBase::OnTickTaskArrivedGroundTruthValid(); //save sample (s)

}



void NNMovementRequestHandle::OnPredictionArrive(){
    //prediction from nn might be displayed in a widget for example.


    //move to requested actor space
    //draw?

}


bool NNMovementRequestHandle::PrepareTaskFor(FNNRequestPackage *package){
    if(!package){
        return false;
    }
    MovementPredictionTask *taskPtr = GetTask();
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





