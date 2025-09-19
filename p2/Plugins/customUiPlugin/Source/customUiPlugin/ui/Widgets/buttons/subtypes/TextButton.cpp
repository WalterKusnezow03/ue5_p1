


#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "GameCore/DebugHelper.h"
#include "customUiPlugin/ui/Widgets/buttons/colors/UiColors.h"
#include "TextButton.h"



UcustomUiComponentBase *UTextButton::DuplicateWidgetInitialized(UObject *parent){
    if(parent){
        UTextButton *newButton = NewWidgetInitialized<UTextButton>(parent);
        if(newButton){
            newButton->SetText(GetText());
            return newButton;
        }
    }
    return nullptr;
}

void UTextButton::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    createTextAndAddToButton();
}

void UTextButton::createTextAndAddToButton(){
    if(TextBlock == nullptr){
        TextBlock = NewObject<UWidgetSlateText>(this);
        SetText("button");
    }
}

void UTextButton::SetText(FString textIn){
    textInternalCopy = textIn;
    if(TextBlock != nullptr){
        TextBlock->SetText(textIn);
    }
}

FString UTextButton::GetText(){
    return textInternalCopy;
}

bool UTextButton::CompareText(FString textIn){
    return textIn == textInternalCopy;
}

/**
 * debug
 */
bool UTextButton::dispatchClick(){
    if(!markedVisible()){
        return false;
    }
    if(TextBlock){
        if(TextBlock->dispatchClick()){
            UiDebugHelper::showScreenMessage("UTextButton Click", FColor::Green);
            TriggerCallback();
            return true;
        }
    }

    return false;
}


void UTextButton::Tick(float deltatime){
    Super::Tick(deltatime);
    if (TextBlock)
    {
        TextBlock->Tick(deltatime);
    }
}