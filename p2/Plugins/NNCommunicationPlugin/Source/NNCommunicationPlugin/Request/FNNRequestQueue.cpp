#include "FNNRequestQueue.h"
#include "DebugPlugin/DebugHelper.h"

FNNRequestPackage *FNNRequestQueue::Find(FString name){
    for (int i = 0; i < tasks.Num(); i++){
        FNNRequestPackage &current = tasks[i];
        if(current.IsSame(name)){
            return &current;
        }
    }
    return nullptr;
}

//notifies the front task and pop fronts it since its finished.
void FNNRequestQueue::NotifyPopFront(const TArray<uint8> &predictedData){
    if(tasks.Num() > 0){
        tasks[0].NotifyAll(predictedData);
        PopFront();
        DebugHelper::logMessage("FNNRequestQueue::NotifyPopFront");
    }
}

void FNNRequestQueue::PopFront(){
     if(tasks.Num() > 0){
        tasks.RemoveAt(0);
        DebugHelper::logMessage("FNNRequestQueue::PopFront");
    }
}



bool FNNRequestQueue::HasTasks(){
    return tasks.Num() > 0;
}

FNNRequestPackage *FNNRequestQueue::frontPackage(){
    if(HasTasks()){
        return &tasks[0];
    }
    return nullptr;
}