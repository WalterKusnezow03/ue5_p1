#include "PlayerControllerWidgetInteractiveBase.h"
#include "DebugPlugin/DebugHelper.h"

APlayerControllerWidgetInteractiveBase::APlayerControllerWidgetInteractiveBase() : Super() {

}


void APlayerControllerWidgetInteractiveBase::BeginPlay(){
    Super::BeginPlay();
    SetupWidgetInteractionComponentOnBeginPlay();
}

void APlayerControllerWidgetInteractiveBase::SetupWidgetInteractionComponentOnBeginPlay(){
    if(!interactionComponent){
        interactionComponent = UAnyMeshWidgetInteractionComponent::MakeInstance(
            GetWorld(),
            this
        );
    }
}

void APlayerControllerWidgetInteractiveBase::Tick(float DeltaTime){
    Super::Tick(DeltaTime);


}

void APlayerControllerWidgetInteractiveBase::SetWidgetInteractionComponentActive(bool flag){
    if(interactionComponent){
        interactionComponent->SetInteractionActive(flag);
    }
}


void APlayerControllerWidgetInteractiveBase::SetWidgetInteractionComponentHoverActive(bool flag){
    if(interactionComponent){
        interactionComponent->SetInteractionHoverActive(flag);
    }
}



void APlayerControllerWidgetInteractiveBase::leftMouseUp(){
    Super::leftMouseUp();
    WidgetInteractionOnLeftMouseUp();
}

void APlayerControllerWidgetInteractiveBase::WidgetInteractionOnLeftMouseUp(){
    if(interactionComponent){
        if(interactionComponent->IsInteractionActive()){
            //DebugHelper::logMessage("APlayerControllerWidgetInteractiveBase::Raycast Test");
            FVector origin = CameraLocation();
            FVector direction = playerLookDir();
            bool result = interactionComponent->RayIntersect(
                origin,
                direction
            );
            /*if(result){
                DebugHelper::logMessage("APlayerControllerWidgetInteractiveBase::Raycast Sucess");
            }*/
        }
    }
}