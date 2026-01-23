#include "AnyMeshWidgetInteractionComponent.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"


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



void UAnyMeshWidgetInteractionComponent::UpdateRay(
    const FVector &origin,
    const FVector &direction
){
    originSaved = origin;
    dirSaved = direction;
}

void UAnyMeshWidgetInteractionComponent::TriggerRayIntersect(){
    RayIntersect(originSaved, dirSaved);
}


bool UAnyMeshWidgetInteractionComponent::RayIntersect(
    const FVector &origin,
    const FVector &direction
){
    if(!isEnabled){
        return false;
    }
    if(ACustomMeshUIActor *found = RayIntersectFound(origin, direction)){
        found->RayIntersect(origin, direction);
        return true;
    }
    
    return false;
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

        //Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
        
        bool bHit = world->LineTraceSingleByChannel(HitResult, start, end, ECC_GameTraceChannel1, Params);

        

        // If the raycast hit something, save hitresult and return positive
        if (bHit){
            if(AActor *actor = HitResult.GetActor()){
                if(ACustomMeshUIActor *casted = Cast<ACustomMeshUIActor>(actor)){
                    DebugHelper::showLineBetween(GetWorld(), origin, HitResult.ImpactPoint, FColor::Red);
                    
                    /*#if WITH_EDITOR
                    DebugHelper::logMessage(
                        FString::Printf(
                            TEXT("UAnyMeshWidgetInteractionComponent::Hit3DUIWidget %s"),
                            *found->GetDebugName() //EDITOR ONLY
                        )
                    );
                    #endif*/


                    return casted;
                }else{
                    DebugHelper::showLineBetween(GetWorld(), origin, HitResult.ImpactPoint, FColor::Blue);
                }
            }
        }
    }
    return nullptr;
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
        if(ACustomMeshUIActor *found = RayIntersectFound(origin, direction)){
            found->RayIntersectHover(origin, direction);
        }
    }
}