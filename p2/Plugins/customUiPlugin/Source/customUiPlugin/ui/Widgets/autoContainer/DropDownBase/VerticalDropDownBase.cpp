#include "VerticalDropDownBase.h"

void UVerticalDropDownBase::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createLayout();
}

void UVerticalDropDownBase::Debug(){
    for (int i = 0; i < 3; i++){
        UTextButton *button = NewWidgetInitialized<UTextButton>(this);
        if(button){
            FString text = FString::Printf(TEXT("item %d"), i);
            button->setText(text);
            AddChild(button);
        }
    }
}





bool UVerticalDropDownBase::dispatchClick(){

    //handle menu show hide
    if(topSelection){
        if(topSelection->dispatchClick()){
            SwitchMenuVisibilty();
            return true;
        }
    }


    //handle sub menu click
    if(selectableList){
        int indexHit = -1;
        bool dispatched = selectableList->dispatchClick(indexHit);
        if(dispatched){
            IBaseUiInterface *interface = selectableList->BaseInterfaceAtIndex(indexHit);
            if(interface && topSelection){

                
                UcustomUiComponentBase *copy = CreateDuplicate(interface); //isA BaseUiInterface

                if(copy){
                    //replace widget in top bar.
                    topSelection->ReplaceChild(0, copy); //replace first index.
                    selectedItem = interface;
                }
            }
        }
        return dispatched;
    }

    return false;
}


UcustomUiComponentBase *UVerticalDropDownBase::CreateDuplicate(IBaseUiInterface *interface){
    if(interface){
        UcustomUiComponentBase *casted = Cast<UcustomUiComponentBase>(interface);
        if(casted){
            UcustomUiComponentBase *duplicate = casted->DuplicateWidgetInitialized(this);
            if(duplicate){
                return duplicate;
            }
        }
    }
    return nullptr;
}

void UVerticalDropDownBase::createLayout(){
    if(!topSelection){
        
        topSelection = NewWidgetInitialized<UHbox>(this);
        if(topSelection && !topTextButton){
            topTextButton = NewWidgetInitialized<UTextButton>(this);
            if(topTextButton){
                topTextButton->setText("Select...");
                topSelection->AddChild(topTextButton);
            }
        }
        Super::AddChild(topSelection);
        
    }
    if(!selectableList){
        
        selectableList = NewWidgetInitialized<UVbox>(this);
        Super::AddChild(selectableList);
        
    }
}



void UVerticalDropDownBase::SwitchMenuVisibilty(){
    if(selectableList){
        bool show = !selectableList->markedVisible();
        selectableList->SetVisible(show);
    }
}



void UVerticalDropDownBase::AddChild(IBaseUiInterface *item){
    createLayout();
    if(selectableList){
        selectableList->AddChild(item);
    }
}




