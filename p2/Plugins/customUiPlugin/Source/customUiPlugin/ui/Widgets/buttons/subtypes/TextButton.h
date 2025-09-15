#pragma once


#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "customUiPlugin/ui/Widgets/callback/callback.h"
#include "customUiPlugin/ui/Widgets/buttons/ButtonBase.h"
#include "Components/TextBlock.h"

#include "TextButton.generated.h"



UCLASS()
class CUSTOMUIPLUGIN_API UTextButton : public UButtonBase
{
    GENERATED_BODY()

public:
    virtual void init() override;

    void setText(FString textIn);
    FString getText();

    virtual bool dispatchClick() override;

    bool CompareText(FString textIn);

    virtual UcustomUiComponentBase *DuplicateWidgetInitialized(UObject *parent) override;

private:
    UPROPERTY()
    UTextBlock *TextBlock = nullptr;
    
    FString textInternalCopy;
    void createTextAndAddToButton();

    void setUpCallbackHover();
    void setDesignHovered();
    void setDesignDefault();
};