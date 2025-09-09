#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/screens/WorldToScreenOverlays/item/WorldMarker.h"

#include "WorldMarkerCanvas.generated.h"

class UWorldMarkerComponent;

///@brief will save any kind of "WidgetSlateWrapperBase" tied to a world position
///and render it to thu UCanvasScreen
///can for example display interaction keys "E to pickup etc.."
/// ---> Will tick world marker manually, no other widget add needed here!
UCLASS()
class CUSTOMUIPLUGIN_API UWorldMarkerCanvas : public UCanvasScreen{
    GENERATED_BODY()
private:
    bool blogMessage = false;

protected:
    virtual void PostInitProperties() override;
    void FindPlayerControllerAndWorld();
    bool PlayerControllerValid();



public:
    ///@brief tick to update the items based on player transform
    virtual void Tick(float deltatime) override;

    virtual bool dispatchClick() override;


    // --- only use this interface to add new markers to the canvas ---

    ///@brief adds a marker to the canvas, OWNERSHIP TAKEN!
    void AddMarker(UWorldMarker *marker);

    //add from aactor component
    void AddMarker(UWorldMarkerComponent *component);



    ///--- markers wont be removed until explicitly removed! ---

    //removes a marker again - designed for entity markers to remove them selves.
    //make sure that the interface pointer and uwidget pointer are still valid when
    //removing from here.
    void RemoveMarker(UWorldMarker *marker);




private:
    UWorldMarker *findMarker(IBaseUiInterface *item);

    bool CanAddMarker(UWorldMarker *marker);
    

    UPROPERTY()
    TArray<UWorldMarker *> trackedMarkers;

    bool inScreenSpace(
        const FVector &WorldLocation,
        FVector2D &result
    );


    // -- tick --
    void Update(UWorldMarker *marker);

    // -- cache --
    
    UWorld *worldPtr = nullptr;

    UPROPERTY()
    APlayerController *Playercontroller = nullptr;

    FVector playerLocationCache;
};