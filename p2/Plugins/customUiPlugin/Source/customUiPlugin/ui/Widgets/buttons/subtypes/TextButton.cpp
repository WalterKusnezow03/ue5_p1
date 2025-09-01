


#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "GameCore/DebugHelper.h"
#include "customUiPlugin/ui/Widgets/buttons/colors/UiColors.h"
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
    textInternalCopy = textIn;
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}

FString UTextButton::getText(){
    return textInternalCopy;
}

bool UTextButton::CompareText(FString textIn){
    return textIn == textInternalCopy;
}

/**
 * debug
 */
bool UTextButton::dispatchClick(){
    bool found = Super::dispatchClick();
    if(found){
        DebugHelper::showScreenMessage(FString::Printf(TEXT("PRESSED BUTTON: %s"), *textInternalCopy), FColor::Orange);
    }
    return found;
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



    //Debug
    if(false){
        DebugHelper::logMessage("dispatch cursor DEBUG!");
        dispatchClick();
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
