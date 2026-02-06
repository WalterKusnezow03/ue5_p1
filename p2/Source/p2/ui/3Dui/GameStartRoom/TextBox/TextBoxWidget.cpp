#include "TextBoxWidget.h"

#include "customuipluginbase/Dispatcher/ClickDispatcher.h"

//override
void UTextBoxWidget::Tick(float DeltaTime){

}

bool UTextBoxWidget::dispatchClick(const FVector2D &position){
    ClickDispatcher dispatcher;
    if(dispatcher.InBound(this, position)){
        isSelected = true;
        return true;
    }
    isSelected = false;
    return false;
}

bool UTextBoxWidget::dispatchHover(const FVector2D &position){
    return false;
}



UTextBlock *UTextBoxWidget::GetTextBlockWidget(){
    if(UWidget *raw = GetTextWidget()){
        if(UTextBlock *casted = Cast<UTextBlock>(raw)){
            return casted;
        }
    }
    return nullptr;
}


FString UTextBoxWidget::GetText(){
    if(UTextBlock *text = GetTextBlockWidget()){
        return text->GetText().ToString();
    }
    return TEXT("");
}


void UTextBoxWidget::dispatchUserInput(UserInput &input){
    if(isSelected){
        //porcess type input

        FString current = GetText();
        if (input.PressedBackspace())
        {
            current = current.LeftChop(1);
            SetText(current);
            return;
        }

        FString append = input.GetPressedKeysAsString();
        current += append;
        SetText(current);
    }
}


void UTextBoxWidget::SetText(FString stringin){
    if(UTextBlock *text = GetTextBlockWidget()){
        text->SetText(FText::FromString(stringin));
    }
}


void UTextBoxWidget::ResetTextIfEmpty(){
    if(GetText().Len() <= 0){
        ResetText();
    }
}


void UTextBoxWidget::ResetText(){
    SetText(defaultText);
}