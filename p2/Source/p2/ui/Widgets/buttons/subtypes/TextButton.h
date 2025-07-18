#pragma once


#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "p2/ui/Widgets/callback/callback.h"
#include "p2/ui/Widgets/buttons/ButtonBase.h"
#include "Components/TextBlock.h"

#include "TextButton.generated.h"

class UPlayerUi;


UCLASS()
class UTextButton : public UButtonBase
{
    GENERATED_BODY()

public:
    virtual void init() override;

    void setText(FString textIn);


    virtual bool dispatchClick() override;

private:

    UTextBlock *TextBlock = nullptr;
    FString textInternalCopy;
    void createTextAndAddToButton();

    void setUpCallbackHover();
    void setDesignHovered();
    void setDesignDefault();
};