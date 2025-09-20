

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/ScaleBox.h"
#include "GameCore/DebugHelper.h"
#include "customUiPlugin/ui/Widgets/callback/callback.h"

#include "ButtonBase.h"



///@brief sets the callback for onlick, REMOVES the old callback!
void UButtonBase::SetCallBack(FSimpleDelegate callbackIn){
    
    //create callback object
    createPressedCallbackIfNeeded();
    if (callbackPointer != nullptr)
    {
        callbackPointer->SetCallback(callbackIn);
    }
    
}

void UButtonBase::TriggerCallback(){
    if(callbackPointer){
        callbackPointer->UCallbackFunction(); //manual trigger
    }
}

void UButtonBase::createPressedCallbackIfNeeded(){
    if(callbackPointer == nullptr){
        callbackPointer = NewObject<UCallback>(this); //only a new one if needed
    }
}





