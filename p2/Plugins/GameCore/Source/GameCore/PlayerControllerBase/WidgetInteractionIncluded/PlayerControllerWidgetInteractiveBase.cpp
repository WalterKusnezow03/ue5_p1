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
        SetInteractionComponentNotifiedInterface();
        SetWidgetInteractionComponentHoverActive(true);
    }
}

void APlayerControllerWidgetInteractiveBase::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    TickInteractionComponent();
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

//inject this actor to the interaction component: will dispatch pointer to widgets
//to notify the player
void APlayerControllerWidgetInteractiveBase::SetInteractionComponentNotifiedInterface(){
    if(interactionComponent){
        interactionComponent->SetCallbackForDelayedInteractions(this);
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

void APlayerControllerWidgetInteractiveBase::TickInteractionComponent(){
    if(interactionComponent){
        if(interactionComponent->IsHoverActive()){
            DispatchInteractKeyPressedStateToInteractionComponent();
            FVector origin = CameraLocation();
            FVector direction = playerLookDir();
            interactionComponent->TickHovered(
                origin,
                direction
            );
        }
    }   
}


void APlayerControllerWidgetInteractiveBase::DispatchInteractKeyPressedStateToInteractionComponent(){
    if(interactionComponent){
        bool holdInteractKey = playerInputContainer.interactKeyIsHoldDown();
        interactionComponent->TickInteractKeyHoldDown(holdInteractKey);
    }
}
    


void APlayerControllerWidgetInteractiveBase::ReceiveCallback(UWidgetInteractPayload *payload){
    //NOTHING - FOR SUB CLASSES IN P2 GAME
}