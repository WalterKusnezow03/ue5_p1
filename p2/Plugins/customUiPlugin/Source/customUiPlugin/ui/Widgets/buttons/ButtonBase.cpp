

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
    Super::init();
    createButton();
}


void UButtonBase::createScaleBox(){
    if(!scalebox){
        scalebox = NewObject<UScaleBox>(this);
        scalebox->SetStretch(EStretch::ScaleToFit);
    }
}


void UButtonBase::createButton(){
    createScaleBox();
    if(!button){
        button = NewObject<UButton>(this);
    }

    if(button && scalebox){
        scalebox->SetStretch(EStretch::ScaleToFit); //Skaliert den Text automatisch --- ScaleToFill odr ScaleToFit
        button->AddChild(scalebox); //so button sichtbar

        WAS_INIT_FLAG = true;
        // scalebox->AddChild(button); //kleiner test, sichtbar aber anderer inhalt von scale box nicht mehr

        
    }
}

/**
 * ---- click dispatch manual -----
 */


/// @brief tests if this button was hit on click!
/// @return 
bool UButtonBase::dispatchClick(){
    if(!markedVisible()){
        return false;
    }
    //DebugHelper::logMessage("UButtonBase try dispatch click");
    if (button) //may be marked invisible: dont do hittest
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
                TriggerCallback();
                // DebugHelper::logMessage("UButtonBase dispatch try sucess");

                for(int i = 0; i < 10; i++){
                    DebugHelper::showScreenMessage("UButtonBase dispatch try sucess");
                }


                return true;
            }
            
            if(false){
                FString message = FString::Printf(
                    TEXT("UButtonbase dispatch no sucess (%.2f, %.2f)(%.2f %.2f), cursor(%.2f, %.2f)"),
                    topLeft.X, topLeft.Y,
                    bottomRight.X, bottomRight.Y,
                    cursorPos.X, cursorPos.Y
                );
                DebugHelper::logMessage(message);
            }
            
        }
    }

    return false;
}

/**
 * ---- pressed callback unreal default ----
 */

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


//not in use, doesnt fix button not interacting sometimes...
void UButtonBase::reloadCallback(){
    if(callbackPointer != nullptr && button != nullptr){
        button->OnClicked.RemoveDynamic(callbackPointer, &UCallback::UCallbackFunction); //delete old, replace!
        button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);
    }
}





