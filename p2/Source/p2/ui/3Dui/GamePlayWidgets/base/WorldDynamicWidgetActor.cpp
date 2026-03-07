#include "WorldDynamicWidgetActor.h"
#include "p2/entityManager/EntityManager.h"
#include "DebugPlugin/DebugHelper.h"


void AWorldDynamicWidgetActor::BeginPlay(){
    Super::BeginPlay();
    wasReleased = true; //mark free.
    UpdateVisibilityAndCollision(true);
}

void AWorldDynamicWidgetActor::InitFromObjectPool(){
    if(wasReleased){
        wasReleased = false;
        UpdateVisibilityAndCollision(true);
    }
}

void AWorldDynamicWidgetActor::InitFromObjectPool(
    USceneComponent *attachTo,
    const FVector &location
){
    if(wasReleased){
        InitFromObjectPool();
        AttachToSceneWithRelativeLocation(attachTo, location);
    }
}



void AWorldDynamicWidgetActor::ReleaseToObjectPool(){
    if(wasReleased){
        return;
    }
    wasReleased = true;
    DetachFromScene();
    UpdateVisibilityAndCollision(false);

    if(EntityManager *e = AworldLevel::entityManager()){
        e->add(this);
    }
}

EWorldDynamicWidgetEnum AWorldDynamicWidgetActor::GetWidgetType(){
    return EWorldDynamicWidgetEnum::ENone;
}



void AWorldDynamicWidgetActor::AttachToSceneWithRelativeLocation(
    USceneComponent *attachTo,
    const FVector &location
){
    AttachToScene(attachTo);
    SetActorRelativeLocation(location);
}

void AWorldDynamicWidgetActor::AttachToScene(USceneComponent *attachTo){
    if(attachTo){
        //NEVER CALLED! WHY!
        AttachToComponent(attachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        DebugHelper::logMessage("AWorldDynamicWidgetActor::AttachToScene");
    }
}

void AWorldDynamicWidgetActor::DetachFromScene(){
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

