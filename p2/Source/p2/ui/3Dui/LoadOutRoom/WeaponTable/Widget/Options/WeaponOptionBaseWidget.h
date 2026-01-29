#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/BorderInterfaceUtil.h"
#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"

#include "WeaponOptionBaseWidget.generated.h"


class UBorder;

/// @brief base widget for weapon attachment selection and weapon type
UCLASS()
class P2_API UWeaponOptionBaseWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTextWidget();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetBorderWidget();

   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSetting")
    FLinearColor colorBase = FLinearColor::Black;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSetting")
    FLinearColor colorChecked = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSetting")
    FLinearColor colorHover = FLinearColor::Black;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetSetting")
    float desiredPadding = 20.0f;

    //inits base properties (text from enum type, padding, visibility)
    virtual void Init();

    virtual void UpdateTextFromType();
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

protected:
    bool bIsChecked = false;
    bool bMarkedVisible = true;

    UBorder *GetBorderWidgetCasted();
    void UpdatePadding();

    void InitBorderInterface();

    BorderInterfaceUtil borderInterface;
};
