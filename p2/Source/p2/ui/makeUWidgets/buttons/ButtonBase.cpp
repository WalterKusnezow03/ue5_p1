

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/ScaleBox.h"
#include "GameCore/DebugHelper.h"
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
    
    if(button && scalebox){
        scalebox->SetStretch(EStretch::ScaleToFit); // Skaliert den Text automatisch --- ScaleToFill odr ScaleToFit
        button->AddChild(scalebox); //so button sichtbar

        WAS_INIT_FLAG = true;
        // scalebox->AddChild(button); //kleiner test, sichtbar aber anderer inhalt von scale box nicht mehr
        

    }

    

}

/**
 * ---- pressed callback ----
 */

///@brief sets the callback for onlick, REMOVES the old callback!
void UButtonBase::SetCallBack(FSimpleDelegate callbackIn){
    if(button != nullptr){
        //create callback object
        createPressedCallbackIfNeeded();
        if (callbackPointer != nullptr)
        {
            callbackPointer->SetCallback(callbackIn);
        
            //reagiert besser
            button->SetClickMethod(EButtonClickMethod::MouseDown);
            //button->OnReleased.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

            button->OnClicked.RemoveDynamic(callbackPointer, &UCallback::UCallbackFunction); //delete old, replace!
            button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

            button->SetIsEnabled(true);

            //schlechter
            //button->SetClickMethod(EButtonClickMethod::DownAndUp);//button reagiert ohne schlecht...
            //button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

            DebugHelper::logMessage("debugCallback created"); //printed
        
        }
    }
}

void UButtonBase::createPressedCallbackIfNeeded(){
    if(callbackPointer == nullptr){
        callbackPointer = NewObject<UCallback>(button); //only a new one if needed
    }
}


//not in use, doesnt fix button not interacting sometimes...
void UButtonBase::reloadCallback(){
    if(callbackPointer != nullptr && button != nullptr){
        button->OnClicked.RemoveDynamic(callbackPointer, &UCallback::UCallbackFunction); //delete old, replace!
        button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);
    }
}



void UButtonBase::makeTransparent(){
    if(button){
        button->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)); // Vollständig transparent
    }
    
}


/**
 * --- hovered callback ---
 */


void UButtonBase::SetCallBackOnHovered(
    FSimpleDelegate onHoveredDelegate,
    FSimpleDelegate onUnHoveredDelegate
){
    if(button != nullptr){
        createHoveredAndUnHoveredCallbackIfNeeded();

        //"OnHovered, OnUnhovered" in doc
        if(callbackPointerOnHovered != nullptr){
            callbackPointerOnHovered->SetCallback(onHoveredDelegate);

            button->OnHovered.RemoveDynamic(
                callbackPointerOnHovered, 
                &UCallback::UCallbackFunction
            ); //delete old, replace!
            button->OnHovered.AddDynamic(
                callbackPointerOnHovered, 
                &UCallback::UCallbackFunction
            );
        }
        if(callbackPointerOnUnHovered != nullptr){
            callbackPointerOnUnHovered->SetCallback(onUnHoveredDelegate);

            button->OnUnhovered.RemoveDynamic(
                callbackPointerOnUnHovered, 
                &UCallback::UCallbackFunction
            ); //delete old, replace!
            button->OnUnhovered.AddDynamic(
                callbackPointerOnUnHovered, 
                &UCallback::UCallbackFunction
            );
        }
        

    }
    
}

void UButtonBase::createHoveredAndUnHoveredCallbackIfNeeded(){
    //create callback object
    if(button){
        if(callbackPointerOnHovered == nullptr){
            callbackPointerOnHovered = NewObject<UCallback>(button); //only a new one if needed
        }
        if(callbackPointerOnUnHovered == nullptr){
            callbackPointerOnUnHovered = NewObject<UCallback>(button); //only a new one if needed
        }
    }
    
}

