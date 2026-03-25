#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"


#include "HudUiWidget.generated.h"

class FPlayerStatusInventorySlot;
class UMiniMapWidget;
class UInventorySlotWidget;

UCLASS()
class P2_API UHudUiWidget : public UUserWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:

    // -- blueprint api --
    //widget functions to get the check boxes, check selected Mode
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetHealthText();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetAmmunitionText();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetTopMessage();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetMiniMap();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetInventorySlotPanel();

    // update widget elements
    void UpdateWidget(FPlayerStatus &playerStatusStruct);

    //void Tick(float deltatime);

    // --- needed for minimap update ---
    virtual void Tick(float DeltaTime) override;
    virtual bool dispatchClick(const FVector2D &position) override {return false;}

	//MUST BE OVERRIDEN!!! - UWidget derived can return itself.
	virtual UWidget *baseLayoutPointer() override {
        return this;
    }

    bool InitMiniMap(AActor *player);

    void RegisterActorToMiniMap(AActor *actor, EMarkerType type);
    void UnRegisterActorFromMiniMap(AActor *actor);

protected:
    UMiniMapWidget *MiniMapCasted();


    void UpdateHealthText(FString text);
    void UpdateHealthText(FText text);

    void UpdateAmmunitionText(FString text);
    void UpdateAmmunitionText(FText text);

    void UpdateInventorySlots(TArray<FPlayerStatusInventorySlot> &slots);
};
