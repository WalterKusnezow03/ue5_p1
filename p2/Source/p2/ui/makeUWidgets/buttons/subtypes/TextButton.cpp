

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "p2/DebugHelper.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "TextButton.h"


TextButton::TextButton(){
    TextBlock = nullptr;
}

TextButton::TextButton(UPlayerUi &uiRef){
    TextBlock = nullptr;
    saveParent(uiRef);
    createButton(); //super
    createTextAndAddToButton();


}

TextButton::~TextButton(){
    button = nullptr;
    TextBlock = nullptr;
}


void TextButton::createTextAndAddToButton(){
    if(playerUiParent != nullptr && button != nullptr){
        TextBlock = NewObject<UTextBlock>(playerUiParent);
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

void TextButton::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}




