#pragma once


#include "CoreMinimal.h"
#include "GameCore/PlayerControllerBase/InteractionCallbackInterface/WidgetInteractionCallbackInterface.h"

#include "AnyMeshWidgetInteractionComponent.generated.h"

class ACustomMeshUIActor;


/// Custom AcustomMeshUiActor Intraction Component
UCLASS()
class GAMECORE_API UAnyMeshWidgetInteractionComponent : public USceneComponent
{
	GENERATED_BODY()
public:
    UAnyMeshWidgetInteractionComponent();

    static UAnyMeshWidgetInteractionComponent *MakeInstance(
        UWorld *world,
        AActor *attachto
    );
    void AttachTo(AActor *actor);

    virtual void BeginPlay() override;
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction
    ) override;


    // -- Ray Setup and Trigger --
    bool RayIntersect(
        const FVector &origin,
        const FVector &direction
    );
    void TickHovered(
        const FVector &origin,
        const FVector &direction
    );

    // -- Enable/ Disable --
    void SetInteractionActive(bool enabled);
    bool IsInteractionActive();

    void SetInteractionHoverActive(bool enabled);
    bool IsHoverActive();



    // --- Interaction hold and down and delayed callback ---
    // -- dispatch interact key hold down "E" --
    void TickInteractKeyHoldDown(bool holdInteractKey);

    //the player controller will register here, the pointer will
    //be dispatched to widgets to notify player if needed.
    void SetCallbackForDelayedInteractions(
        IWidgetInteractionCallbackInterface *interfaceIn
    );

private:
    IWidgetInteractionCallbackInterface *notifyInterface = nullptr;
    ACustomMeshUIActor *latestHitUiActor = nullptr;

    /// @brief remove latest hit ui actor and remove interface from
    /// the ui actor. Needed for safety reasons to not spread spointers across
    /// all widgets
    void EjectLatestHitUiActor();

    FVector dirSaved;
    FVector originSaved;

    UPROPERTY()
    AActor *parent = nullptr;

    bool isEnabled = true;
    bool isHoverEnabled = false;

    bool bDrawDebugLine = false;

    ACustomMeshUIActor *RayIntersectFound(
        const FVector &origin,
        const FVector &direction
    );
};