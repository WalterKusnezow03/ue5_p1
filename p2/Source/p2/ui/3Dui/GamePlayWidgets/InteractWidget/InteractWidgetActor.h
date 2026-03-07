#pragma once

#include "CoreMinimal.h"

#include "p2/ui/3Dui/GamePlayWidgets/base/WorldDynamicWidgetActor.h"

#include "InteractWidgetActor.generated.h"


class UInteractWidget;

/// @brief press e to interact widget, timer based.
/// ----> TOOD: SLATE CUSTOM UI CIRCULAR?
UCLASS()
class GAMECORE_API AInteractWidgetActor : public AWorldDynamicWidgetActor {
    GENERATED_BODY()

public:
    /*static AInteractWidgetActor *MakeInstance(
        UWorld *world,
        USceneComponent *attachTo,
        FVector relativeLocation
    );*/

    //reset / init method from entity manager
    virtual void InitFromObjectPool() override;
    virtual void ReleaseToObjectPool() override;

    //to be called by interact widget and no one else.
    void Notify();

    //to be called by owning entity to completly disable or enable
    void SetInteractWidgetActive(bool flag);
    void SetPayload(AActor *actor);

    virtual EWorldDynamicWidgetEnum GetWidgetType() override {
        return EWorldDynamicWidgetEnum::EInteractWidget;
    }
   
    

private:
    bool isActive = true; //active flag for notfiy
    AActor *payloadActor = nullptr;

protected:
    

    virtual void CreateWidgetMeshData() override;

    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    void InitInteractWidgetOnBeginPlay();
    UInteractWidget *GetInteractWidgetCasted();
};