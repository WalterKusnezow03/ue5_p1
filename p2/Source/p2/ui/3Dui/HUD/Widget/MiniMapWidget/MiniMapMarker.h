#pragma once

#include "CoreMinimal.h"
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"

#include "MiniMapMarker.generated.h"

class UCanvasPanelSlot;

// -- only visual representation, no aactor attached here --
UCLASS()
class P2_API UMiniMapMarker : public UUserWidget {
    GENERATED_BODY()

public:
    void Init(UCanvasPanelSlot *slot);

    

    //hide unhide, cached by minimap widget to reassign
    bool MarkedFree();
    void MarkFree(bool flag);

    //switches the imga eof the marker.
    void SetType(EMarkerType type);
    void UpdateLocation(const FVector2D &target);

    void UpdateTypeAndLocation(EMarkerType type, const FVector2D &target);

    /*
    EMarkerType::
    EEnemy,
    EPlayer,
    EWeapon,
    */

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetNeutralMarker();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetEnemyMarker();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetPlayerMarker();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWeaponMarker();

protected:
    UWidget *GetMarker(EMarkerType type);

    UCanvasPanelSlot *slot = nullptr;
    bool bMarkedFree = true;
};