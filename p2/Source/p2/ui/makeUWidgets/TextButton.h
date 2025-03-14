#pragma once


#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "Components/TextBlock.h"

class UPlayerUi;

class TextButton : public customUiComponentBase
{
public:
    TextButton();
    TextButton(UPlayerUi &uiRef);
    ~TextButton();

    virtual UWidget *baseLayoutPointer() override{
        return button;
    }

    void setText(FString textIn);

    UButton *buttonPoiner(){
        return button;
    }

    void SetCallBack(FSimpleDelegate callbackIn);

private:
    UButton *button = nullptr;
    UTextBlock *TextBlock = nullptr;

    void createButton();



    UCallback *callbackPointer = nullptr;
};