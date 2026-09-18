#include "NNHeatMapActor.h"


ANNHeatMapActor::ANNHeatMapActor() : ARenderTargetActorBase()
{
	
	PrimaryActorTick.bCanEverTick = true;

}

void ANNHeatMapActor::MakeInstance(UWorld *world){
    AActorBase::TMakeInstance<ANNHeatMapActor>(world);
}

void ANNHeatMapActor::MakeInstanceAttachTo(AActor *actor, FVector offset){
    if(actor){
        USceneComponent *rootPtr = actor->GetRootComponent();
        MakeInstanceAttachTo(rootPtr, offset);
    }
}

void ANNHeatMapActor::MakeInstanceAttachTo(USceneComponent* TargetComponent, FVector Offset)
{
    if (TargetComponent)
    {
        ANNHeatMapActor* Made = AActorBase::TMakeInstance<ANNHeatMapActor>(TargetComponent->GetWorld());
        if (Made)
        {
            FAttachmentTransformRules AttachmentRules(
                EAttachmentRule::SnapToTarget, // Location
                EAttachmentRule::SnapToTarget, // Rotation
                EAttachmentRule::KeepWorld,    // Scale
                true                           // Weld Simulated Bodies
            );

            // Attach to the USceneComponent directly
            Made->AttachToComponent(TargetComponent, AttachmentRules);
            
            Made->GetRootComponent()->SetRelativeLocation(Offset);
        }
    }
}



void ANNHeatMapActor::BeginPlay(){
    Super::BeginPlay();

    if(MeshToRenderOn){
        // Reihenfolge: Pitch (Y), Yaw (Z), Roll (X)
        MeshToRenderOn->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
        DebugHelper::logMessage("ANNHeatMapActor::Set Rotation");
    }

    //register to pathfinde nn interface

    NNPathFinderExtensionApi::SubscribeHeatMapListener(this);

    if(RenderedTexture){
        RenderedTexture->UpdateImageData(&widgetImageData);
    }
}


void ANNHeatMapActor::EndPlay(const EEndPlayReason::Type EndPlayReason){

    Super::EndPlay(EndPlayReason);
}