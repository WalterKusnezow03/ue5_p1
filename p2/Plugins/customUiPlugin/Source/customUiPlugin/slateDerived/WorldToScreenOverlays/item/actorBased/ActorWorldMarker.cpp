#include "ActorWorldMarker.h"

void UActorWorldMarker::Tick(float deltatime){
    Super::Tick(deltatime);
    if(attachedActor != nullptr){
        FVector actorLocation = attachedActor->GetActorLocation();
        UpdatePosition(actorLocation);
    }
}

void UActorWorldMarker::SetActor(AActor *actor){
    attachedActor = actor;
}


void UActorWorldMarker::MarkEntityCollected(){
    isEnabled = false;
    SetVisible(false);
}

void UActorWorldMarker::MarkEntityAlive(){
    isEnabled = true;
    SetVisible(true);
}