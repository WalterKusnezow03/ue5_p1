

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/ScaleBox.h"
#include "p2/DebugHelper.h"
#include "p2/ui/makeUWidgets/callback/callback.h"

#include "ButtonBase.h"



void UButtonBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    createButton();
}




void UButtonBase::createButton(){
    button = NewObject<UButton>(this); 

    scalebox = NewObject<UScaleBox>(this);
    scalebox->SetStretch(EStretch::ScaleToFit); // Skaliert den Text automatisch --- ScaleToFill odr ScaleToFit

    button->AddChild(scalebox); //so button unsichtbar
    //scalebox->AddChild(button); //kleiner test, sichtbar aber anderer inhalt von scale box nicht mehr

}

///@brief sets the callback for onlick
void UButtonBase::SetCallBack(FSimpleDelegate callbackIn){
    if(button != nullptr){
        //create callback object
        callbackPointer = NewObject<UCallback>(button);
        if(callbackPointer != nullptr){
            callbackPointer->SetCallback(callbackIn);
        }

        //reagiert besser
        button->SetClickMethod(EButtonClickMethod::MouseDown);
        //button->OnReleased.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

        button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

        button->SetIsEnabled(true);

        //schlechter
        //button->SetClickMethod(EButtonClickMethod::DownAndUp);//button reagiert ohne schlecht...
        //button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

        DebugHelper::logMessage("debugCallback created"); //printed
    }
}


void UButtonBase::reloadCallback(){
    if(callbackPointer != nullptr && button != nullptr){
        button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);
    }
}



