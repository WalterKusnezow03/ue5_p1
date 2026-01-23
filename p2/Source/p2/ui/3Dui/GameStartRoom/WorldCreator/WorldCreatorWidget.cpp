#include "WorldCreatorWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"


void UWorldCreatorWidget::SetParent(AGameStartRoom *parentIn){
    parent = parentIn;
}

void UWorldCreatorWidget::ResetParent(){
    parent = nullptr;
}

void UWorldCreatorWidget::NotifyLaunch(){
    if(UTextBoxWidget *box = GetTextBoxWidget()){
        if(parent){
            parent->TryCreateWorld(box->GetText()); //copy text from text box
        }
    }
}


UTextBoxWidget *UWorldCreatorWidget::GetTextBoxWidget(){
    if(UWidget *raw = GetTextWidget()){
        if(UTextBoxWidget *casted = Cast<UTextBoxWidget>(raw)){
            return casted;
        }
    }
    return nullptr;
}



//override
bool UWorldCreatorWidget::dispatchClick(const FVector2D &position){
    ClickDispatcher dispatcher;

    bool result = false;
    if (UTextBoxWidget *box = GetTextBoxWidget())
    {
        if(box->dispatchClick(position)){
            
            //other options

            result = true;
        }
    }
    if(UWidget *button = GetPlayButton()){
        if(dispatcher.InBound(button, position)){
            NotifyLaunch();
            result = true;
        }
    }
    if(parent){
        parent->NotifyOnClickDispatch();
    }


    return false; 
}

bool UWorldCreatorWidget::dispatchHover(const FVector2D &position){

    return false;
}

bool UWorldCreatorWidget::TextBoxIsActive(){
    if(UTextBoxWidget *widget = GetTextBoxWidget()){
        return widget->IsMarkedSelected();
    }
    return false;
}