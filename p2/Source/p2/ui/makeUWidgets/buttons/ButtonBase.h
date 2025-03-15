#pragma once


#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

class UPlayerUi;

/**
 * button with scaled content
 * 
 * button (base)
 * -->scale box (scales content to parent)
 * --->content inside scale box
 * 
 */
class ButtonBase : public customUiComponentBase
{
public:
    ButtonBase();
    ButtonBase(UPlayerUi &uiRef);
    ~ButtonBase();

    virtual UWidget *baseLayoutPointer() override{
        return button; //button //scalebox(wrong)
    }

    void SetCallBack(FSimpleDelegate callbackIn);


protected:
    UButton *button = nullptr;
    UScaleBox *scalebox = nullptr; //inside button. Use to add your childs

    UCallback *callbackPointer = nullptr;

    void createButton();



    
};