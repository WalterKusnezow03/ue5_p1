#pragma once

#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"
#include "GameCore/Ui3D/InteractionComponent/AnyMeshWidgetInteractionComponent.h"


#include "PlayerControllerWidgetInteractiveBase.generated.h"

UCLASS()
class GAMECORE_API APlayerControllerWidgetInteractiveBase :
public APlayerControllerBase {
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

    UPROPERTY()
    UAnyMeshWidgetInteractionComponent *interactionComponent = nullptr;

    void SetWidgetInteractionComponentActive(bool flag);

    void SetWidgetInteractionComponentHoverActive(bool flag);

    void TickInteractionComponent();
};