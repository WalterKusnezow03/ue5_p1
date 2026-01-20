#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"


#include "WeaponOptionBaseWidget.generated.h"


class UBorder;

/// @brief base widget for weapon attachment selection and weapon type
UCLASS()
class P2_API UWeaponOptionBaseWidget : public UUserWidget{
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
    float desiredPadding = 20.0f;

    //inits base properties (text from enum type, padding, visibility)
    virtual void Init();

    virtual void UpdateTextFromType();
    void SetText(FString textIn);
    void SetVisible(bool flag);

    void SetChecked(bool flag);

protected:
    bool bIsChecked = false;

    UBorder *GetBorderWidgetCasted();
    void UpdatePadding();
};
