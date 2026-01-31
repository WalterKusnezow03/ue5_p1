#pragma once

#include "CoreMinimal.h"


#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/MiniMapData.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "MiniMapMarker.h"
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"

#include "MiniMapWidget.generated.h"

class USizeBox;
class UCanvasPanel;

//owns all markers and actors can subsribe by type, and unsubscribe
UCLASS()
class P2_API UMiniMapWidget : public UUserWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:

    /// -- TSubclass for markers --
    UPROPERTY(EditAnywhere, Category="WidgetSetting")
    TSubclassOf<UMiniMapMarker> widgetClassForItem;

    /// -- end subclass for markers --

    /// -- ufunction getter --
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetBaseCanvas();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetSizeBox();




    void AddMarker(EMarkerType type, AActor *actor);
    void RemoveMarker(AActor *actor);

    // player setup
    void SetPlayerReference(AActor *playerIn);

    // --- needed for update ---
    virtual void Tick(float DeltaTime) override;
    virtual bool dispatchClick(const FVector2D &position) override {return false;}

	//MUST BE OVERRIDEN!!! - UWidget derived can return itself.
	virtual UWidget *baseLayoutPointer() override {
        return this;
    }

protected:

    AActor *player = nullptr;
    MiniMapData miniMapData;

    USizeBox *GetSizeBoxCasted();
    UCanvasPanel *GetBaseCanvasCasted();
    
    UMiniMapMarker *CreateMarker(EMarkerType type, const FVector2D &pos);
    UMiniMapMarker *CreateMarker(EMarkerType type);
    TArray<UMiniMapMarker *> GetAllMarkerChildren();
    UMiniMapMarker *FindMarker();
    void AddMarkerToCanvasAndInit(UMiniMapMarker *marker);

    void MarkAllMarkersFree();

    bool GetMiniMapResolution(FVector2D &outRes);

    void UpdateMarkers();
    void UpdateMarkers(const std::map<EMarkerType, TArray<FVector2D>> &mapIn);
    void UpdateMarkers(EMarkerType type, const TArray<FVector2D> &array);

    void UpdatePlayerTransformToMinimapData();


};