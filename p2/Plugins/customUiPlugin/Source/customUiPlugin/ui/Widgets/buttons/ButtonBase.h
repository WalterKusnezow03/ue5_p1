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
 * button base - meant as abstract class.
 */

UCLASS()
class CUSTOMUIPLUGIN_API UButtonBase : public UcustomUiComponentBase
{
    GENERATED_BODY()

public:

    void SetCallBack(FSimpleDelegate callbackIn);

protected:
    void TriggerCallback();

    UPROPERTY()
    UCallback *callbackPointer = nullptr; //callback on click

    void createPressedCallbackIfNeeded();
   


};