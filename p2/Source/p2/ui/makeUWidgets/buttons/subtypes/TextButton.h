#pragma once


#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "p2/ui/makeUWidgets/buttons/ButtonBase.h"
#include "Components/TextBlock.h"

class UPlayerUi;

class TextButton : public ButtonBase
{
public:
    TextButton();
    TextButton(UPlayerUi &uiRef);
    ~TextButton();

    void setText(FString textIn);


private:

    UTextBlock *TextBlock = nullptr;
    void createTextAndAddToButton();

    
};