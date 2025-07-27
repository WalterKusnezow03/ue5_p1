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
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override{
        return button; //button //scalebox(wrong)
    }

    void SetCallBack(FSimpleDelegate callbackIn);

    void reloadCallback();
    void makeTransparent();

    void SetCallBackOnHovered(
        FSimpleDelegate onHoveredDelegate,
        FSimpleDelegate onUnHoveredDelegate
    );

    //NEW
    virtual bool dispatchClick() override;

protected:

    UButton *button = nullptr;
    UScaleBox *scalebox = nullptr; //inside button. Use to add your childs

    UPROPERTY()
    UCallback *callbackPointer = nullptr;

    UPROPERTY()
    UCallback *callbackPointerOnHovered = nullptr;

    UPROPERTY()
    UCallback *callbackPointerOnUnHovered = nullptr;

    void createButton();
    void createPressedCallbackIfNeeded();
    void createHoveredAndUnHoveredCallbackIfNeeded();

    void disableUMGClicks();
};