#pragma once

#include "CoreMinimal.h"

#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"

#include "InteractWidgetActor.generated.h"


class UInteractWidget;

/// @brief press e to interact widget, timer based.
/// ----> TOOD: SLATE CUSTOM UI CIRCULAR?
UCLASS()
class GAMECORE_API AInteractWidgetActor : public ACustomMeshUIActor {
    GENERATED_BODY()

public:
    static AInteractWidgetActor *MakeInstance(
        UWorld *world,
        USceneComponent *attachTo,
        FVector relativeLocation
    );

    //to be called by interact widget and no one else.
    void Notify();

    //to be called by owning entity to completly disable or enable
    void SetInteractWidgetActive(bool flag);
    void SetPayload(AActor *actor);

private:
    bool isActive = true; //active flag for notfiy
    AActor *payloadActor = nullptr;

protected:
    void AttachToSceneWithRelativeLocation(
        USceneComponent *attachTo,
        const FVector &location
    );
    void AttachToScene(USceneComponent *attachTo);

    virtual void CreateWidgetMeshData() override;

    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    void InitInteractWidgetOnBeginPlay();
    UInteractWidget *GetInteractWidgetCasted();
};