#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "HudUiWidget.generated.h"

UCLASS()
class P2_API UHudUiWidget : public UUserWidget {
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

    // update widget elements
    void UpdateWidget(FPlayerStatus &playerStatusStruct);

    //void Tick(float deltatime);

private:
    void UpdateHealthText(FString text);
    void UpdateHealthText(FText text);

    void UpdateAmmunitionText(FString text);
    void UpdateAmmunitionText(FText text);
};
