#include "ActorTrajectoryTracker.h"

void ActorTrajectoryTracker::AddSubscriber(INotifyActorTrajectoryTrackerChanged *interface){
    if(interface && !subscribed.Contains(interface)){
        subscribed.Add(interface);
    }
}


void ActorTrajectoryTracker::RemoveSubscriber(INotifyActorTrajectoryTrackerChanged *interface){
    if(interface){
        subscribed.Remove(interface);
    }
}


void ActorTrajectoryTracker::OnTrajectoryAdded(){
    ActorTrajectoryTrackerBase::OnTrajectoryAdded();

    //notify world space
    Trajectory copied;
    if(collection.GetLastWorldSpace(copied)){
        for (int i = 0; i < subscribed.Num(); i++){
            if(INotifyActorTrajectoryTrackerChanged *current = subscribed[i]){
                current->OnTrajectoryAddedWorldSpace(copied);
            }
        }
    }

    //notify local space - to be implemented
    //may be not needed.
}