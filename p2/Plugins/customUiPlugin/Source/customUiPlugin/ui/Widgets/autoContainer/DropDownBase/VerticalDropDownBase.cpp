#include "VerticalDropDownBase.h"

#include "customUiPlugin/ui/Widgets/autoContainer/sizing/FixedSizeBox.h"

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
            button->SetText(text);
            AddChild(button);
        }
    }
}





///@brief dispatches a click and returns the index of the pressed item inside the 
///picker list, if any hit!
bool UVerticalDropDownBase::dispatchClick(){
    //handle menu show hide
    if(topSelection){
        if(topSelection->dispatchClick()){
            SwitchMenuVisibilty();
            return true;
        }

        //debug
        if(selectedCopy){
            if(UWidget *w = selectedCopy->baseLayoutPointer()){
                UWidgetSlateWrapperBase *casted = Cast<UWidgetSlateWrapperBase>(w);
                if(casted){
                    UiDebugHelper::logMessage(
                        FString::Printf(
                            TEXT("UVerticalDropDownBase top item scale %s, targeted %s"),
                            *casted->GetResolution().ToString(),
                            *FVector2D(topSelectedResX,topResY).ToString()
                        )
                    );
                }else{
                    UiDebugHelper::logMessage("UVerticalDropDownBase item not castable");
                }
            }else{
                UiDebugHelper::logMessage("UVerticalDropDownBase item nullptr");
            }
        }
        
    }


    //reset sub menu clicked index
    indexHitFromClickDispatch = -1; //Reset index

    //handle sub menu click
    if(selectableList){
        int indexHit = -1;
        bool dispatched = selectableList->dispatchClick(indexHit);
        if(dispatched){
            SelectIndex(indexHit);
            indexHitFromClickDispatch = indexHit;
        }
        return dispatched;
    }

    return false;
}


int UVerticalDropDownBase::ListIndexHitFromClickDispatch(){
    return indexHitFromClickDispatch;
}

IBaseUiInterface *UVerticalDropDownBase::BaseInterfaceFromListAtIndex(int index){
    if(selectableList){
        return selectableList->BaseInterfaceAtIndex(index);
    }
    return nullptr;
}





void UVerticalDropDownBase::SelectIndex(int indexHit){
    if(!selectableList){
        return;
    }
    IBaseUiInterface *interface = selectableList->BaseInterfaceAtIndex(indexHit);
    if(interface && topSelection){

        UcustomUiComponentBase *copy = CreateDuplicate(interface); //isA BaseUiInterface
        if(copy){
            
            //replace widget in top bar.
            topSelection->ReplaceChild(0, copy); //replace first index.
            selectedItem = interface;

            //resize
            selectedCopy = copy;
            selectedCopy->enableTicklog();
            RescaleAsTopItem(selectedCopy);
        }
    }
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
        CreateTopSelectionBar();
        CreateSpacer(20);
    }
    
    if(!selectableList){
        selectableList = NewWidgetInitialized<UVbox>(this);
        Super::AddChild(selectableList);
    }
}

void UVerticalDropDownBase::CreateTopSelectionBar(){
    if(!topSelection){   
        topSelection = NewWidgetInitialized<UHbox>(this);
        Super::AddChild(topSelection);
    }

    

    if(topSelection && !topTextButton){
        topTextButton = NewWidgetInitialized<UTextButton>(this);
        if(topTextButton){
            RescaleAsTopItem(topTextButton);
            topTextButton->SetText("Select...");
            topSelection->AddChild(topTextButton);


            //debug
            if(UWidgetSlateText *t = topTextButton->SlateTextBlock()){
                //t->enableTicklog();
            }
            
        }
    }
    if(topSelection && !topArrow){
        topArrow = NewObject<UWidgetArrowBase>(this); //points to right by default
        topArrow->AddState(90.0f);
        topArrow->AddState(-90.0f);
        topSelection->AddChild((IBaseUiInterface*) topArrow);

        topArrow->SetResolution(FVector2D(topResY, topResY));
    }
    
}



void UVerticalDropDownBase::SwitchMenuVisibilty(){
    if(selectableList){
        bool show = !selectableList->markedVisible();
        selectableList->SetVisible(show);
    }
}

void UVerticalDropDownBase::HidePickerMenu(){
    if(selectableList){
        if(selectableList->markedVisible()){
            SwitchMenuVisibilty();
        }
    }
}




void UVerticalDropDownBase::AddChild(IBaseUiInterface *item){
    createLayout();
    if(selectableList){
        RescaleAsListItem(item);
        selectableList->AddChild(item);
    }
}




const TArray<IBaseUiInterface *> &UVerticalDropDownBase::AccessListInternalItemsTmp() const{
    if(selectableList){
        return selectableList->AccessInternalItemsTmp();
    }
    return fallbackArray;
}





// rescaling helper

void UVerticalDropDownBase::RescaleAsTopItem(IBaseUiInterface *item){
    RescaleIfPossible(item, FVector2D(topSelectedResX, topResY));
}
void UVerticalDropDownBase::RescaleAsListItem(IBaseUiInterface *item){
    RescaleXIfPossible(item, topSelectedResX + topResY); //arrow button is squared anyway
}
   

void UVerticalDropDownBase::RescaleIfPossible(IBaseUiInterface *item, FVector2D scale){
    if(item){
        UWidgetSlateWrapperBase *casted = Cast<UWidgetSlateWrapperBase>(item);
        if(casted){
            casted->SetResolution(scale);
        }else{
            UWidget *base = item->baseLayoutPointer();
            casted = Cast<UWidgetSlateWrapperBase>(base);
            if(casted){
                casted->SetResolution(scale);
                UiDebugHelper::logMessage("UVerticalDropDownBase rescaled item");
            }
        }
    }
}


void UVerticalDropDownBase::RescaleXIfPossible(IBaseUiInterface *item, int scaleX){
    if(item){
        UWidgetSlateWrapperBase *casted = Cast<UWidgetSlateWrapperBase>(item);
        if(casted){
            casted->SetResolutionXUniform(scaleX);
        }else{
            UWidget *base = item->baseLayoutPointer();
            casted = Cast<UWidgetSlateWrapperBase>(base);
            if(casted){
                casted->SetResolutionXUniform(scaleX);
            }
        }
    }
}