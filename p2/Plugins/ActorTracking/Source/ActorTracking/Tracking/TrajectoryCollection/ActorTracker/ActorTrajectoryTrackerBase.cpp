#include "ActorTrajectoryTrackerBase.h"


void ActorTrajectoryTrackerBase::Setup(AActor *actor, float intervall, int listMaxSize){
    collection.Setup(listMaxSize);
    
    intervallSaved = std::abs(intervall);
    timer.Begin(intervallSaved, true);
    trackedActor = actor;
    FillListOnSetup();
}

void ActorTrajectoryTrackerBase::FillListOnSetup(){
    if(trackedActor){
        FVector location = trackedActor->GetActorLocation();
        for (int i = 0; i < collection.NumMax(); i++)
        {
            collection.Add(location);
        }
    }
}

void ActorTrajectoryTrackerBase::FlagVisible(){
    isVisible = true;
}

void ActorTrajectoryTrackerBase::SetUseVisibility(bool inFlag){
    useVisibilityFlag = inFlag;
}
    

AActor *ActorTrajectoryTrackerBase::GetActor(){
    return trackedActor;
}

void ActorTrajectoryTrackerBase::Tick(float deltatime){
    if(trackedActor){
        if(useVisibilityFlag){
            if(!isVisible){
                return;
            }
        }

        if(timer.TickWithTimesUpReset(deltatime)){
            FVector location = trackedActor->GetActorLocation();
            collection.Add(location);
            OnTrajectoryAdded();

            //mark as invisible again
            //which will be used if Visibilty Flag is requested
            if(useVisibilityFlag){
                isVisible = false; //must be set to visible externally again
            }
        }
    }
}

void ActorTrajectoryTrackerBase::OnTrajectoryAdded(){

}

bool ActorTrajectoryTrackerBase::IsFlaggedVisibleReset(){
    bool copy = isVisible;
    isVisible = false;
    return copy;
}

FVector ActorTrajectoryTrackerBase::ActorLocation(){
    if(trackedActor){
        return trackedActor->GetActorLocation();
    }
    return FVector(0, 0, 0);
}

TArray<Trajectory> ActorTrajectoryTrackerBase::localTrajectories() const {
    TArray<Trajectory> outArray;
    if(trackedActor){
        FVector actorLocation = trackedActor->GetActorLocation();
        outArray = collection.ToArrayLocalSpace(actorLocation);
    }
    return outArray;
}


TArray<Trajectory> ActorTrajectoryTrackerBase::worldTrajectories() const {
    TArray<Trajectory> outArray;
    if(trackedActor){
        outArray = collection.ToArray();
    }
    return outArray;
}


TArray<Trajectory> ActorTrajectoryTrackerBase::worldTrajectoriesNormalizedTime() const {
    TArray<Trajectory> outArray;
    if(trackedActor){
        outArray = collection.ToArrayNormalizedTime();
    }
    return outArray;
}