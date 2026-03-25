#include "WorldCreatorWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/GameStartRoom/GameStartRoom.h"

void UWorldCreatorWidget::SetParent(AGameStartRoom *parentIn){
    parent = parentIn;
    Init();
}

void UWorldCreatorWidget::ResetParent(){
    parent = nullptr;
}

void UWorldCreatorWidget::NotifyLaunch(){
    if(UTextBoxWidget *box = GetTextBoxWidget()){
        if(parent){
            parent->TryCreateWorld(box->GetText()); //copy text from text box
            box->ResetText();
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

    //lock player movement based on text selected flag
    if(parent){
        parent->NotifyOnClickDispatch();
    }


    return false; 
}

bool UWorldCreatorWidget::dispatchHover(const FVector2D &position){
    bool result = false;
    if (playButtonUtil.dispatchHover(position)){
        result = true;
    }
    if(UTextBoxWidget *widget = GetTextBoxWidget()){
        if(widget->dispatchHover(position)){
            result = true;
        }
    }

    return result;
}

//remove hover on widget left
void UWorldCreatorWidget::removeHover(){
    playButtonUtil.removeHover();
    if(UTextBoxWidget *widget = GetTextBoxWidget()){
        return widget->removeHover();
    }
}



bool UWorldCreatorWidget::TextBoxIsActive(){
    if(UTextBoxWidget *widget = GetTextBoxWidget()){
        return widget->IsMarkedSelected();
    }
    return false;
}

void UWorldCreatorWidget::dispatchUserInput(UserInput &input){
    if(TextBoxIsActive()){
        if(UTextBoxWidget *found = GetTextBoxWidget()){
            found->dispatchUserInput(input);
        }
    }
}


void UWorldCreatorWidget::Init(){
    SetupFromDefaultColors(
        playButtonUtil,
        GetPlayButton()
    );
}

