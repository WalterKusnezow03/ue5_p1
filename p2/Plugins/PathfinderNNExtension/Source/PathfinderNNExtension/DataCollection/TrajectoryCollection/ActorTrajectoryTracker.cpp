#include "ActorTrajectoryTracker.h"


void ActorTrajectoryTracker::Setup(AActor *actor, float intervall, int listMaxSize){
    collection.Setup(listMaxSize);
    
    intervallSaved = std::abs(intervall);
    timer.Begin(intervallSaved, true);
    trackedActor = actor;
    FillListOnSetup();
}

void ActorTrajectoryTracker::FillListOnSetup(){
    if(trackedActor){
        FVector location = trackedActor->GetActorLocation();
        for (int i = 0; i < collection.NumMax(); i++)
        {
            collection.Add(location);
        }
    }
}

void ActorTrajectoryTracker::FlagVisible(){
    isVisible = true;
}

AActor *ActorTrajectoryTracker::GetActor(){
    return trackedActor;
}

void ActorTrajectoryTracker::Tick(float deltatime){
    if(trackedActor){
        if(isVisible){
            //track
            isVisible = false;
        }

        if(timer.TickWithTimesUpReset(deltatime)){
            FVector location = trackedActor->GetActorLocation();
            collection.Add(location);

            //mark as invisible again

        }
    }
}

bool ActorTrajectoryTracker::IsFlaggedVisible(){
    return isVisible;
}

FVector ActorTrajectoryTracker::ActorLocation(){
    if(trackedActor){
        return trackedActor->GetActorLocation();
    }
    return FVector(0, 0, 0);
}

TArray<Trajectory> ActorTrajectoryTracker::localTrajectories() const {
    TArray<Trajectory> outArray;
    if(trackedActor){
        FVector actorLocation = trackedActor->GetActorLocation();
        outArray = collection.ToArrayLocalSpace(actorLocation);
    }
    return outArray;
}


TArray<Trajectory> ActorTrajectoryTracker::worldTrajectories() const {
    TArray<Trajectory> outArray;
    if(trackedActor){
        outArray = collection.ToArray();
    }
    return outArray;
}