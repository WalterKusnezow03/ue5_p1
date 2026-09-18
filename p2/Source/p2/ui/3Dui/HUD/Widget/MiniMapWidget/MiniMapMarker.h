#pragma once

#include "CoreMinimal.h"
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"
#include "Blueprint/UserWidget.h"
#include "StoragePlugin/Storage/ImageData/Image/Image.h"

#include "MiniMapMarker.generated.h"

class UCanvasPanelSlot;
class FMiniMapMarkerTransform;
class UWidgetPixelDraw;

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
    void UpdateRotation(float angle);
    void UpdateTransform(const FMiniMapMarkerTransform &inTransform);
    
    void UpdateCustomMarkerImage(
        const Image *image,
        float scale = 1.0f
    );
    void UpdateCustomMarkerImage(
        const Image &image,
        float scale = 1.0f
    );

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

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetFlagRedMarker();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetFlagWhiteMarker();

    //custom draw marker
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetCustomMarker();

    

protected:
    UWidget *GetMarker(EMarkerType type);
    UWidgetPixelDraw *GetCustomMarkerCasted();

    UCanvasPanelSlot *slot = nullptr;
    bool bMarkedFree = true;
};