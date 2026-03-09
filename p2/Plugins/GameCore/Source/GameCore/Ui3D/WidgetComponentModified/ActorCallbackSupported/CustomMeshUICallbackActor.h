#pragma once

#include "CoreMinimal.h"

#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "GameCore/PlayerControllerBase/InteractionCallbackInterface/WidgetInteractionCallbackInterface.h"
#include "GameCore/PlayerControllerBase/InteractionCallbackInterface/WidgetInteractPayload.h"


#include "CustomMeshUICallbackActor.generated.h"

/**
 * UI Actor that contains a Dynamic Mesh 3D Widget Component.
 * Supports callback interaction, with or without payload
 */
UCLASS()
class GAMECORE_API ACustomMeshUICallbackActor : public ACustomMeshUIActor 
{
	GENERATED_BODY()

public:
    void ClearAllCallacksAndPayload();
    void NotifyAllCallbacks();

    void NotifyAllCallbacks(UWidgetInteractPayload *payload);

public:
    // ---- PLAYER TEMPORARY CALLBACK FOR WIDGETS IF HOVERED ----
    
    // --- API AnyMeshWidgetInteractionComponent ---
    // ---- callback interface injection for p2 game widgets if needed ----
	// if a widget has processed work it can notify this interface via
	// this parent which will own a AnyMeshWidget and its UWidget.
	// (Do NOT break the hierachy of ACustomMeshUiActor->AnyMeshWidgetComp->UWidget)
    // ----> Inject main interface here: also is resettet if widget is not hovered anymore 
	void SetCallbackForDelayedInteractions(IWidgetInteractionCallbackInterface *interfaceIn);
	
    //overriden to reset callback interface if not hovered anymore.
    bool RayIntersectHover(
        const FVector &origin,
        const FVector &direction
    ) override;
    // --- API AnyMeshWidgetInteractionComponent ---

    void SetPayloadByPointer(UWidgetInteractPayload *payload);

protected:
    UWidgetInteractPayload *injectedPayload = nullptr;
    bool HasPayload();

protected:
    //resets main notify interface if not hovered anymore
    void ResetWidgetInteractionCallbackIfNotHovered(); //clear player callback
    void ClearPlayerCallback();
    IWidgetInteractionCallbackInterface *widgetInteracionCallbackInterfacePlayer = nullptr;
    AActor *payloadActorForPlayer = nullptr;

    // ---- PLAYER CALLBACK FOR WIDGETS ---- END ----

public:
    // ---- CALLBACK OTHER INJECTED INTERFACES: persist if not removed ---- 
    //(designed for owning entities of this widget)
    void AddPersistentCallbackInterface(
        IWidgetInteractionCallbackInterface *interfaceIn
    );
    
    

protected:
    void NotifyInjectedInterfaces();
    void ClearPersistentCallbackInterfaceBuffer();
    TArray<IWidgetInteractionCallbackInterface *> injectedPersistentCallbacksNoPayload;
};