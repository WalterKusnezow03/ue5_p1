#pragma once


#include "CoreMinimal.h"
#include "AnyMeshWidgetInteractionComponent.generated.h"


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
    void UpdateRay(
        const FVector &origin,
        const FVector &direction
    );
    void TriggerRayIntersect();
    bool RayIntersect(
        const FVector &origin,
        const FVector &direction
    );

    // -- Enable/ Disable --
    void SetInteractionActive(bool enabled);
    bool IsInteractionActive();

private:
    FVector dirSaved;
    FVector originSaved;

    UPROPERTY()
    AActor *parent = nullptr;

    bool isEnabled = true;
};