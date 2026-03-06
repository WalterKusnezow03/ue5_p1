#pragma once

#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"
#include "GameCore/Ui3D/InteractionComponent/AnyMeshWidgetInteractionComponent.h"
#include "GameCore/PlayerControllerBase/InteractionCallbackInterface/WidgetInteractionCallbackInterface.h"

#include "PlayerControllerWidgetInteractiveBase.generated.h"

/// @brief includes AnyMeshWidgetInteraction Component and callback interface for custom widgets
UCLASS()
class GAMECORE_API APlayerControllerWidgetInteractiveBase :
public APlayerControllerBase,
public IWidgetInteractionCallbackInterface
{
    GENERATED_BODY()

public:
    APlayerControllerWidgetInteractiveBase();
    


protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;


    // ---- Custom Widget Interaction ----
    virtual void leftMouseUp() override;
    void WidgetInteractionOnLeftMouseUp();
    void SetupWidgetInteractionComponentOnBeginPlay();

    void DispatchInteractKeyPressedStateToInteractionComponent();

    UPROPERTY()
    UAnyMeshWidgetInteractionComponent *interactionComponent = nullptr;

    void SetWidgetInteractionComponentActive(bool flag);

    void SetWidgetInteractionComponentHoverActive(bool flag);

    void TickInteractionComponent();

private:
    void SetInteractionComponentNotifiedInterface();

    // callback from 3D world widgets
public:
    virtual void ReceiveCallback(AActor *payloadActor) override;
};