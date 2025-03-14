

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "p2/DebugHelper.h"
#include "p2/ui/makeUWidgets/callback/callback.h"
#include "TextButton.h"


TextButton::TextButton(){
    button = nullptr;
    TextBlock = nullptr;
}

TextButton::TextButton(UPlayerUi &uiRef){
    button = nullptr;
    TextBlock = nullptr;
    saveParent(uiRef);
    createButton();
}

TextButton::~TextButton(){
    button = nullptr;
    TextBlock = nullptr;
}


void TextButton::createButton(){
    if(playerUiParent != nullptr){
        button = NewObject<UButton>(playerUiParent);
        TextBlock = NewObject<UTextBlock>(playerUiParent);
        button->AddChild(TextBlock);
        setText("button");
    }
}

void TextButton::setText(FString textIn){
    if(TextBlock != nullptr){
        TextBlock->SetText(FText::FromString(textIn));
    }
}





void TextButton::SetCallBack(FSimpleDelegate callbackIn){
    //create callback object
    callbackPointer = NewObject<UCallback>(playerUiParent);

    DebugHelper::logMessage("debugCallback try create"); //printed

    if(callbackPointer != nullptr){
        callbackPointer->SetCallback(callbackIn);

        //debug execute
        /*if (callbackIn.IsBound())  // Ensure the delegate is bound before executing
        {
            callbackIn.Execute();
            DebugHelper::logMessage("debugCallback debug execute"); //printed
        }*/



        if(button != nullptr){
            button->OnClicked.AddDynamic(callbackPointer, &UCallback::UCallbackFunction);

            DebugHelper::logMessage("debugCallback created"); //printed
        }
    }
}

