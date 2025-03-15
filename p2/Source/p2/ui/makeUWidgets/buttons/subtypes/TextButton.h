#pragma once


#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "p2/ui/makeUWidgets/buttons/ButtonBase.h"
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


private:

    UTextBlock *TextBlock = nullptr;
    void createTextAndAddToButton();

    
};