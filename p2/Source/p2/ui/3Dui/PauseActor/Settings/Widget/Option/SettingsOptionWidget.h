#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/BorderInterfaceUtil.h"
#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"

#include "SettingsOptionWidget.generated.h"


class UBorder;

/// @brief base widget for weapon attachment selection and weapon type
UCLASS()
class P2_API USettingsOptionWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTextWidget();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetBorderWidgetEnabled();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetBorderWidgetDisabled();
   

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSetting")
    float desiredPadding = 20.0f;

    //inits base properties (text from enum type, padding, visibility)
    virtual void Init();

   
    void SetText(FString textIn);
   

    void SetChecked(bool flag);


    // --- override IBaseUiInterface ---
    virtual void Tick(float DeltaTime) override;
    virtual bool dispatchClick(const FVector2D &position) override;

    virtual bool dispatchHover(const FVector2D &position) override;
    virtual bool markedVisible() { return bMarkedVisible; }
    virtual void SetVisible(bool flag) override;

	virtual UWidget *baseLayoutPointer() override {
        return this;
    }

    //remove hover on widget left
    virtual void removeHover() override;

    bool IsChecked(){
        return bIsChecked;
    }

protected:
    bool bIsChecked = false;
    bool bMarkedVisible = true;

    UBorder *GetBorderWidgetCastedEnabled();
    UBorder *GetBorderWidgetCastedDisabled();
    void UpdatePadding();

    void InitBorderInterface();

    BorderInterfaceUtil borderInterfaceEnabled;
    BorderInterfaceUtil borderInterfaceDisabled;
};
