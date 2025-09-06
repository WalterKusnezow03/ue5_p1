#pragma once 

#include "customUiPlugin/slateDerived/WorldToScreenOverlays/item/WorldMarker.h"

#include "ActorWorldMarker.generated.h"


/// @brief Actor marker can be added to WorldMarker canvas, will track AActor by ptr to 
/// update location manually and no tick from an aatcor instance is needed.
UCLASS()
class CUSTOMUIPLUGIN_API UActorWorldMarker : public UWorldMarker{
    GENERATED_BODY()

public:
    virtual void Tick(float deltatime) override;

    void SetActor(AActor *actor);

    /// @brief marks entity collected in custom GC, will hide the marker.
    void MarkEntityCollected();
    void MarkEntityAlive();

private:

    UPROPERTY()
    AActor *attachedActor;

};