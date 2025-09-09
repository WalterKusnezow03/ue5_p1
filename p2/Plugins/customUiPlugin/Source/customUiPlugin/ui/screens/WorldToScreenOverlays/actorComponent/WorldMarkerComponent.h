#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "customUiPlugin/ui/screens/WorldToScreenOverlays/item/WorldMarker.h"

#include "WorldMarkerComponent.generated.h"

/// @brief derived from UActorComponent, not a scene component, more performance.
UCLASS()
class CUSTOMUIPLUGIN_API UWorldMarkerComponent : public UActorComponent{
    GENERATED_BODY()

    bool bLogMessage = false;

public:
    UWorldMarkerComponent();

    static UWorldMarkerComponent *Make(AActor *actorAttachTo, FVector &localOffset);

    void SetRelativeLocation(FVector &pos);

    ///@brief disables the marker, hides it, and wont update until enabled again.
    void SetEnabled(bool flag);

    //getting marker for modifying uwidget (should happen in createWidget override protected)
    //or adding to worldmarkercanvas
    //-- is property of this actor component and will mark for destroy if this is destroyed! --
    UWorldMarker *GetMarkerInternal();

    void RemoveMarkerFromWorldMarkerCanvas();

protected:
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction
    ) override;

    virtual void PostInitProperties() override;
    virtual void BeginDestroy() override;

    /// @brief function to create a widget for the UWorldMarker, must be overriden from 
    /// subclass for any content to be visible
    /// once the marker is added to the worldmarker canvas the widget cant be changed.
    virtual void CreateWidgetForUWorldMarker(); 

private:
    void CreateMarker();

    FVector relativeLocation;


    UPROPERTY()
    UWorldMarker *MyMarker = nullptr;
};