

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/ScaleBox.h"
#include "GameCore/DebugHelper.h"
#include "customUiPlugin/ui/Widgets/callback/callback.h"

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

        //disableUMGClicks();
    }
}

/**
 * ---- click dispatch manual -----
 */

void UButtonBase::disableUMGClicks(){
    if(button){
        button->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

/// @brief tests if this button was hit on click!
/// @return 
bool UButtonBase::dispatchClick(){

    DebugHelper::logMessage("UButtonBase try dispatch click");
    if (button && markedVisible()) //may be marked invisible: dont do hittest
    {
        TSharedPtr<SWidget> SlateWidget = button->GetCachedWidget();

        if (SlateWidget.IsValid())
        {

            FGeometry Geometry = SlateWidget->GetCachedGeometry();

            FVector2D cursorPos = FSlateApplication::Get().GetCursorPos();

            FVector2D topLeft = Geometry.LocalToAbsolute(FVector2D(0, 0));
            FVector2D bottomRight = Geometry.LocalToAbsolute(Geometry.GetLocalSize());

            FVector2D size = bottomRight - topLeft; //AB = B - A
            topLeft -= size * 0.1f;
            bottomRight += size * 0.1f;

            bool inBoundsX = cursorPos.X >= topLeft.X && cursorPos.X <= bottomRight.X;
            bool inBoundsY = cursorPos.Y >= topLeft.Y && cursorPos.Y <= bottomRight.Y;




            
            /*
            FVector2f topLeft = Geometry.GetAbsolutePosition();
            FVector2f Size = Geometry.GetAbsoluteSize();
            FVector2f bottomRight = topLeft + Size;

            bool inBoundsX = mousePositionIn.X >= topLeft.X && mousePositionIn.X <= bottomRight.X;
            bool inBoundsY = mousePositionIn.Y >= topLeft.Y && mousePositionIn.Y <= bottomRight.Y;
            */
            if(inBoundsX && inBoundsY){
                if(callbackPointer){
                    callbackPointer->UCallbackFunction(); //manual trigger
                }
                DebugHelper::logMessage("UButtonBase dispatch try sucess");

                for(int i = 0; i < 10; i++){
                    DebugHelper::showScreenMessage("UButtonBase dispatch try sucess");
                }


                return true;
            }
            

            FString message = FString::Printf(
                TEXT("UButtonbase dispatch no sucess (%.2f, %.2f)(%.2f %.2f), cursor(%.2f, %.2f)"),
                topLeft.X, topLeft.Y,
                bottomRight.X, bottomRight.Y,
                cursorPos.X, cursorPos.Y
            );
            DebugHelper::logMessage(message);
        }
    }

    return false;
}

/**
 * ---- pressed callback unreal default ----
 */

///@brief sets the callback for onlick, REMOVES the old callback!
void UButtonBase::SetCallBack(FSimpleDelegate callbackIn){
    if(button != nullptr){
        //create callback object
        createPressedCallbackIfNeeded();
        if (callbackPointer != nullptr)
        {
            callbackPointer->SetCallback(callbackIn);
        
            /*
            //DEBUG using custom click dispatcher
            return;

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
            */
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
        button->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.2f)); // Vollständig transparent
    }
    
}


/**
 * --- hovered callback ---
 */


void UButtonBase::SetCallBackOnHovered(
    FSimpleDelegate onHoveredDelegate,
    FSimpleDelegate onUnHoveredDelegate
){
    //Debug
    return;

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

