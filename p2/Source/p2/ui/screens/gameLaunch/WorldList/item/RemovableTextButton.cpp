#include "RemovableTextButton.h"


void URemovableTextButton::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createText();
}

void URemovableTextButton::createText(){
    if(!textButton){
        textButton = NewObject<UTextButton>(this);
        textButton->init();
        AddChild(textButton); // added to left hbox
    }
}

void URemovableTextButton::SetText(FString Text){
    if(textButton){
        textButton->setText(Text);
    }
}

FString URemovableTextButton::GetText(){
    if(textButton){
        return textButton->getText();
    }
    return TEXT("emptyRemovableText");
}

void URemovableTextButton::SetTextButtonCallBack(FSimpleDelegate callbackIn){
    if(textButton){
        textButton->SetCallBack(callbackIn);
    }
}