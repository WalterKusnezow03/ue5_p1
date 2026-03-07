#pragma once

#include "CoreMinimal.h"

#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"
#include "p2/ui/3Dui/GamePlayWidgets/Enum/EWorldDynamicWidgetEnum.h"

#include "WorldDynamicWidgetActor.generated.h"



/// base class for all dynamic widgets in world which can be collected by the gc object pool
/// too many for each actor to have one widget.
/// can be released and inited from entity manager.
UCLASS()
class GAMECORE_API AWorldDynamicWidgetActor : public ACustomMeshUIActor {
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:

    virtual void InitFromObjectPool();
    virtual void InitFromObjectPool(
        USceneComponent *attachTo,
        const FVector &location
    );
    virtual void ReleaseToObjectPool();

    ///MUST BE OVERRIDEN FOR custom GC
    virtual EWorldDynamicWidgetEnum GetWidgetType();

    // --- attach / detach ---

    void AttachToSceneWithRelativeLocation(
        USceneComponent *attachTo,
        const FVector &location
    );
    void AttachToScene(USceneComponent *attachTo);

    void DetachFromScene();

    // --- attach / detach ---


protected:
    bool wasReleased = false;
};