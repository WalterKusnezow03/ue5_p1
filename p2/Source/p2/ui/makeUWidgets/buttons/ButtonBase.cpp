

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/ScaleBox.h"
#include "p2/DebugHelper.h"
#include "p2/ui/makeUWidgets/callback/callback.h"

#include "ButtonBase.h"


ButtonBase::ButtonBase(){
    button = nullptr;
}

ButtonBase::ButtonBase(UPlayerUi &uiRef){
    button = nullptr;
    saveParent(uiRef);
    createButton();
}

ButtonBase::~ButtonBase(){
    button = nullptr;
}


void ButtonBase::createButton(){
    if(playerUiParent != nullptr){
        button = NewObject<UButton>(playerUiParent);
        
        scalebox = NewObject<UScaleBox>(playerUiParent);
        scalebox->SetStretch(EStretch::ScaleToFit); // Skaliert den Text automatisch --- ScaleToFill odr ScaleToFit

        button->AddChild(scalebox); //so button unsichtbar
        //scalebox->AddChild(button); //kleiner test, sichtbar aber anderer inhalt von scale box nicht mehr
    }
}

void ButtonBase::SetCallBack(FSimpleDelegate callbackIn){
    //create callback object
    callbackPointer = NewObject<UCallback>(playerUiParent);

    //DebugHelper::logMessage("debugCallback try create"); //printed

    if(callbackPointer != nullptr){
        callbackPointer->SetCallback(callbackIn);

        if(button != nullptr){

            //reagiert besser
            button->SetClickMethod(EButtonClickMethod::MouseDown);
            button->OnReleased.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

            //schlechter
            //button->SetClickMethod(EButtonClickMethod::DownAndUp);//button reagiert ohne schlecht...
            //button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

            DebugHelper::logMessage("debugCallback created"); //printed
        }
    }
}



