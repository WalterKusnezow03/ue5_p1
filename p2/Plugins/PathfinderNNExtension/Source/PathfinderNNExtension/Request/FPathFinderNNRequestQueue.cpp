#include "FPathFinderNNRequestQueue.h"
#include "DebugPlugin/DebugHelper.h"




void FPathFinderNNRequestQueue::Subscribe(
    IPathfinderNNInterface *interface, 
    AActor *targetActor
){
    if(interface && targetActor){
        if(FPathFinderNNRequestPackage *found = Find(targetActor)){
            found->Subscribe(interface);
        }else{
            FPathFinderNNRequestPackage current(targetActor);    
            current.Subscribe(interface);
            tasks.Add(current);
        }
        //DebugHelper::logMessage("FPathFinderNNRequestQueue::AddInterface");
    }
}

FPathFinderNNRequestPackage *FPathFinderNNRequestQueue::Find(AActor *targetActor){
    if(targetActor){
        for (int i = 0; i < tasks.Num(); i++){
            FPathFinderNNRequestPackage &current = tasks[i];
            if(current.IsSame(targetActor)){
                return &current;
            }
        }
    }
    return nullptr;
}

//notifies the front task and pop fronts it since its finished.
void FPathFinderNNRequestQueue::NotifyPopFront(const TArray<FVector> &predictedPositions){
    if(tasks.Num() > 0){
        tasks[0].NotifyAll(predictedPositions);
        tasks.RemoveAt(0);
        DebugHelper::logMessage("FPathFinderNNRequestQueue::NotifyPopFront");
    }
}


bool FPathFinderNNRequestQueue::HasTasks(){
    return tasks.Num() > 0;
}

AActor *FPathFinderNNRequestQueue::frontActor(){
    if(FPathFinderNNRequestPackage *package = frontPackage()){
        return package->GetActor();
    }
    return nullptr;
}

FPathFinderNNRequestPackage *FPathFinderNNRequestQueue::frontPackage(){
    if(HasTasks()){
        //DebugHelper::logMessage("FPathFinderNNRequestQueue::GetFrontActor");
        return &tasks[0];
    }
    return nullptr;
}