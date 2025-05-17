

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "GameCore/DebugHelper.h"
#include "p2/ui/makeUWidgets/buttons/colors/UiColors.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "TextButton.h"



void UTextButton::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createTextAndAddToButton();
    setUpCallbackHover();
}

void UTextButton::createTextAndAddToButton(){
    if(button != nullptr){
        TextBlock = NewObject<UTextBlock>(this);
        TextBlock->SetAutoWrapText(true);

        /*
        button
        --- scalebox
        ------ text
        */

        scalebox->AddChild(TextBlock);

        setText("button");
    }
}

void UTextButton::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}





/**
 * callback api
 */
void UTextButton::setDesignHovered(){
    if(button){
        button->SetBackgroundColor(UiColors::buttonWhite);
    }
    if(TextBlock){
        TextBlock->SetColorAndOpacity(FSlateColor(UiColors::buttonBlack));
    }
}


void UTextButton::setDesignDefault(){
    if(button){
        makeTransparent(); //super
    }
    if(TextBlock){
        TextBlock->SetColorAndOpacity(FSlateColor(UiColors::buttonWhite));
    }
}


void UTextButton::setUpCallbackHover(){

    SetCallBackOnHovered(
        // on hover
        FSimpleDelegate::CreateLambda([this](){
            if(this){
                this->setDesignHovered();
            } 
        }),

        // on unhover
        FSimpleDelegate::CreateLambda([this](){
            if(this){
                this->setDesignDefault();
            } 
        })
    );

    setDesignDefault();
}
