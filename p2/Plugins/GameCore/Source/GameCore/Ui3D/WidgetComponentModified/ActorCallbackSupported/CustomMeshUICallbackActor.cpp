#include "CustomMeshUICallbackActor.h"







bool ACustomMeshUICallbackActor::RayIntersectHover(
    const FVector &origin,
    const FVector &direction
){
    bool result = Super::RayIntersectHover(origin, direction);
    ResetWidgetInteractionCallbackIfNotHovered();
    return result;
}

//setup callback
void ACustomMeshUICallbackActor::SetCallbackForDelayedInteractions(
    IWidgetInteractionCallbackInterface *interfaceIn
){
    ///sets player notified interface - gets payload.
    widgetInteracionCallbackInterfacePlayer = interfaceIn;
}

void ACustomMeshUICallbackActor::ResetWidgetInteractionCallbackIfNotHovered(){
    if(Widget){
        //hovered by raycast / interaction component -> saved in UInteractionComponentHoveredCache (static shared.)
        if(!Widget->IsMarkedHovered()){
            ClearPlayerCallback();
        }
    }
}

void ACustomMeshUICallbackActor::SetActorPayLoad(AActor *inActor){
    payloadActorForPlayer = inActor;
}

void ACustomMeshUICallbackActor::NotifyAllCallbacks(){
    //notify with payload
    if(widgetInteracionCallbackInterfacePlayer){
        widgetInteracionCallbackInterfacePlayer->ReceiveCallback(payloadActorForPlayer);
    }
    //notify others without payload (may be changed later.)
    for (int i = 0; i < injectedPersistentCallbacksNoPayload.Num(); i++){
        if(IWidgetInteractionCallbackInterface * current = injectedPersistentCallbacksNoPayload[i]){
            current->ReceiveCallback();
        }
    }
}


// ---- Callback other intejected interfaces: persist if not removed ---- 
//(designed for owning entities of this widget)
void ACustomMeshUICallbackActor::AddPersistentCallbackInterface(
    IWidgetInteractionCallbackInterface *interfaceIn
){
    if(interfaceIn){
        if(!injectedPersistentCallbacksNoPayload.Contains(interfaceIn)){
            injectedPersistentCallbacksNoPayload.Add(interfaceIn);
        }
    }
}

void ACustomMeshUICallbackActor::ClearPersistentCallbackInterfaceBuffer(){
    injectedPersistentCallbacksNoPayload.Empty();
}

void ACustomMeshUICallbackActor::ClearPlayerCallback(){
    widgetInteracionCallbackInterfacePlayer = nullptr;
}

void ACustomMeshUICallbackActor::ClearAllCallacks(){
    ClearPersistentCallbackInterfaceBuffer();
    ClearPlayerCallback();
    payloadActorForPlayer = nullptr;
}