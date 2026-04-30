#include "NActorTrajectoryTracker.h"



void NActorTrajectoryTracker::Setup(float intervall, int listmaxSize){
    intervallShared = std::abs(intervall);
    intervallShared = std::max(intervallShared, 0.1f);

    listMaxSizeShared = std::abs(listmaxSize);
    listMaxSizeShared = std::max(listMaxSizeShared, 10);
}



void NActorTrajectoryTracker::AddTrackedActorIfNeeded(AActor *actor){
    if(actor){
        FString name = actor->GetName();
        if(!HasTracked(name)){
            trackerMap[name] = ActorTrajectoryTracker();
            ActorTrajectoryTracker &ref = trackerMap[name];
            ref.Setup(actor, intervallShared, listMaxSizeShared);
        }
    }
}

void NActorTrajectoryTracker::RemoveTrackedActor(AActor *actor){
    if(actor){
        FString name = actor->GetName();
        if(HasTracked(name)){
            trackerMap.erase(name);
        }
    }
    
}

bool NActorTrajectoryTracker::HasTracked(AActor *actor){
    if(actor){
        FString name = actor->GetName();
        return HasTracked(name);
    }
    return false;
}

bool NActorTrajectoryTracker::HasTracked(FString name){
    return trackerMap.find(name) != trackerMap.end();
}

ActorTrajectoryTracker *NActorTrajectoryTracker::FindIfTracked(AActor *actor){
    if(actor){
        FString name = actor->GetName();
        if(HasTracked(name)){
            return &trackerMap[name]; //tmp ptr, do not change map!
        }
    }
    return nullptr;
}

void NActorTrajectoryTracker::Tick(float deltaTime){
    for(auto &pair : trackerMap){
        pair.second.Tick(deltaTime);
    }
}


TArray<Trajectory> NActorTrajectoryTracker::LocalTrajectories(AActor *actor){
    if(ActorTrajectoryTracker *tracker = FindIfTracked(actor)){
        return tracker->localTrajectories();
    }
    TArray<Trajectory> empty;
    return empty;
}


TArray<Trajectory> NActorTrajectoryTracker::worldTrajectories(AActor *actor){
    if(ActorTrajectoryTracker *tracker = FindIfTracked(actor)){
        return tracker->worldTrajectories();
    }
    TArray<Trajectory> empty;
    return empty;
}




void NActorTrajectoryTracker::FlagVisible(AActor *actor){
    if(ActorTrajectoryTracker *tracker = FindIfTracked(actor)){
        tracker->FlagVisible();
    }
}