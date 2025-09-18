#pragma once


#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "customUiPlugin/ui/Widgets/callback/callback.h"
#include "Components/SizeBox.h"
#include "Components/ScaleBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

#include "ButtonBase.generated.h"


/**
 * button with scaled content
 * 
 * button (base)
 * -->scale box (scales content to parent)
 * --->content inside scale box
 * 
 */

UCLASS()
class CUSTOMUIPLUGIN_API UButtonBase : public UcustomUiComponentBase
{
    GENERATED_BODY()

public:
    //call after construct!
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override{
        return scalebox; //button //scalebox(wrong)
    }

    void SetCallBack(FSimpleDelegate callbackIn);

    void reloadCallback();
    


    //NEW
    virtual bool dispatchClick() override;

protected:
    void TriggerCallback();

    UPROPERTY()
    UButton *button = nullptr;

    UPROPERTY()
    UScaleBox *scalebox = nullptr; //inside button. Use to add your childs, macht button sichtbar

    UPROPERTY()
    UCallback *callbackPointer = nullptr; //callback on click

    void createScaleBox();
    void createButton();
    void createPressedCallbackIfNeeded();
   


};