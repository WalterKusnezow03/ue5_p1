

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "p2/DebugHelper.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "TextButton.h"



void UTextButton::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createTextAndAddToButton();
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




