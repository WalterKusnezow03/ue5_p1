// Copyright Walter Kusnezow All Rights Reserved.
#include "AnyMeshWidgetInteractionComponent.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "GameCore/Ui3D/WidgetComponentModified/ActorCallbackSupported/CustomMeshUICallbackActor.h"
#include "GameCore/Ui3D/InteractionComponentCache/InteractionComponentHoveredCache.h"



UAnyMeshWidgetInteractionComponent *UAnyMeshWidgetInteractionComponent::MakeInstance(
    UWorld *world,
    AActor *attachto
){
    if(world && attachto){
        UAnyMeshWidgetInteractionComponent* newComponent = NewObject<UAnyMeshWidgetInteractionComponent>(
            attachto,// Outer MUST be the actor
            UAnyMeshWidgetInteractionComponent::StaticClass(),
            NAME_None,
            RF_Transient // optional, but recommended for runtime-only
        );
        if(newComponent){
            newComponent->RegisterComponent();
            newComponent->AttachTo(attachto);
            return newComponent;
        }
    }
    return nullptr;
}

void UAnyMeshWidgetInteractionComponent::AttachTo(AActor *actor){
    DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    if (actor){
        AttachToComponent(actor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        parent = actor;
    }
}



UAnyMeshWidgetInteractionComponent::UAnyMeshWidgetInteractionComponent(){
    PrimaryComponentTick.bCanEverTick = true;
    SetComponentTickEnabled(true);
}






void UAnyMeshWidgetInteractionComponent::BeginPlay(){
    Super::BeginPlay();

}

void UAnyMeshWidgetInteractionComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction
){
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}




bool UAnyMeshWidgetInteractionComponent::RayIntersect(
    const FVector &origin,
    const FVector &direction
){
    if(!isEnabled){
        return false;
    }
    bool result = false;
    //bDrawDebugLine = true; //draw on click
    if(ACustomMeshUIActor *found = RayIntersectFound(origin, direction)){
        found->RayIntersect(origin, direction);
        result = true;
    }
    //bDrawDebugLine = false; //reset draw on click
    return result;
}


//private raycast
ACustomMeshUIActor *UAnyMeshWidgetInteractionComponent::RayIntersectFound(
    const FVector &origin,
    const FVector &direction
){
    if(UWorld *world = GetWorld()){

        FVector start = origin + direction.GetSafeNormal() * 10.0f; //50cm
        FVector end = origin + direction.GetSafeNormal() * 5000.0f; //50 * 100 = 50m

        // Perform the raycast
        FHitResult HitResult;
        FCollisionQueryParams Params;
        if(parent){
            Params.AddIgnoredActor(parent); // Ignore parent
        }
        Params.bTraceComplex = false; //new lower complexity

        //ECC_GameTraceChannel1 widget Channel
        bool bHit = world->LineTraceSingleByChannel(HitResult, start, end, ECC_GameTraceChannel1, Params);

        

        // If the raycast hit something, save hitresult and return positive
        if (bHit){
            if(AActor *actor = HitResult.GetActor()){
                if(ACustomMeshUIActor *casted = Cast<ACustomMeshUIActor>(actor)){
                    if(bDrawDebugLine){
                        DebugHelper::showLineBetween(GetWorld(), origin, HitResult.ImpactPoint, FColor::Red);
                    }
                    TryInjectInteractCallbakInterfaceTo(casted);

                    return casted;

                }else{
                    if(bDrawDebugLine){
                        DebugHelper::showLineBetween(GetWorld(), origin, HitResult.ImpactPoint, FColor::Blue);
                    }
                    
                }
            }
        }
    }
    return nullptr;
}

void UAnyMeshWidgetInteractionComponent::TryInjectInteractCallbakInterfaceTo(
    ACustomMeshUIActor *raw 
){
    //injects notify interface into hit actor.
    //if the hit actor is not hovered anymore it will eject the
    //interface on its own!
    if(raw && notifyInterface){
        if(ACustomMeshUICallbackActor *callbackCastedActor = Cast<ACustomMeshUICallbackActor>(raw)){
            callbackCastedActor->SetCallbackForDelayedInteractions(notifyInterface);
        }
    }
}






// enable - disable section

void UAnyMeshWidgetInteractionComponent::SetInteractionActive(bool enabled){
    isEnabled = enabled;
    SetHiddenInGame(!isEnabled);
}


bool UAnyMeshWidgetInteractionComponent::IsInteractionActive(){
    return isEnabled;
}


void UAnyMeshWidgetInteractionComponent::SetInteractionHoverActive(bool enabled){
    isHoverEnabled = enabled;
}




// hover ticked
void UAnyMeshWidgetInteractionComponent::TickHovered(
    const FVector &origin,
    const FVector &direction
){
    if(isHoverEnabled && isEnabled){
        
        if (ACustomMeshUIActor *found = RayIntersectFound(origin, direction))
        {
            found->RayIntersectHover(origin, direction);
        }else{
            //update cache even if none found: none is hovered, must be updated
            //too for "cursor leave"
            UInteractionComponentHoveredCache::UpdateHovered(nullptr);
        }
    }
}

bool UAnyMeshWidgetInteractionComponent::IsHoverActive(){
    return isHoverEnabled;
}





void UAnyMeshWidgetInteractionComponent::TickInteractKeyHoldDown(bool holdInteractKey){
    UInteractionComponentHoveredCache::UpdateInteractKeyHoldFlag(holdInteractKey);

    if(false){
        FString prefix = TEXT("UAnyMeshWidgetInteractionComponent::TickInteractKeyHoldDown");
        FString postfix = holdInteractKey ? TEXT("TRUE") : TEXT("FALSE");
        FColor color = holdInteractKey ? FColor::Green : FColor::Red;
        FString message = prefix + postfix;
        DebugHelper::showScreenMessage(message, color);
    }
    
}

//the player controller will register here, the pointer will
//be dispatched to widgets to notify player if needed.
void UAnyMeshWidgetInteractionComponent::SetCallbackForDelayedInteractions(
    IWidgetInteractionCallbackInterface *interfaceIn
){
    notifyInterface = interfaceIn;
}
