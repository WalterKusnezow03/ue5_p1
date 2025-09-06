#include "Vbox.h"
#include "Components/VerticalBoxSlot.h"

void UVbox::init(){
    initVboxIfNeeded();
}

void UVbox::initVboxIfNeeded(){
    if(baseVBox == nullptr){
        baseVBox = NewObject<UVerticalBox>(this);
    }
}



/// @brief if inited!: adds a item to the vbox and custom visibility changes tracking
/// @param item not nullptr
void UVbox::AddChild(IBaseUiInterface *item){
    initVboxIfNeeded(); //if needed for init() skip fallback (not needed to call after construct)
    if(item && baseVBox){
        UWidget *baseOfItem = item->baseLayoutPointer(); //any widget must exist!
        if(baseOfItem){
            Super::AddChild(item); //track in click listeing and visibilty propagation

            //add to hbox
            //baseVBox->AddChildToVerticalBox(baseOfItem);
            AddChild(baseOfItem);
        }
    }
}

/// @brief adds any child to the vbox: NOT Custom click listeing, no custom visibility propagation!
/// @param any 
void UVbox::AddChild(UWidget *any){
    initVboxIfNeeded(); //if needed for init() skip fallback (not needed to call after construct)
    if(any && baseVBox){
        baseVBox->AddChildToVerticalBox(any);
        UpdatePadding(any);
        UpdateAlignment(any);
    }
}


///@brief removes a child from click listening and inner container
void UVbox::RemoveChild(IBaseUiInterface *item){
    if(item){
        Super::RemoveChild(item);
        RemoveChild(item->baseLayoutPointer());
    }
}

///@brief must be overriden
void UVbox::RemoveChild(UWidget *any){
    if(any && baseVBox){
        baseVBox->RemoveChild(any);
    }
}








void UVbox::UpdatePadding(UWidget *widget){
    if(widget){
        if(UVerticalBoxSlot* Slot = Cast<UVerticalBoxSlot>(widget->Slot)){
            Slot->SetPadding(makePadding());
        }
    }
}



// -- Alignment vbox exclusive --

void UVbox::SetItemsFillHorizontal(){
    fillHorizontal = true;
    UpdateAlignmentForAllTrackedItems();
}

void UVbox::UpdateAlignmentForAllTrackedItems(){
    for (int i = 0; i < attachedItems.Num(); i++){
        UcustomUiComponentBase *current = attachedItems[i];
        if(current){
            UWidget *layoutPtr = current->baseLayoutPointer();
            if(layoutPtr){
                UpdateAlignment(layoutPtr);
            }
        }
    }
}

/// @brief --- DOES NOT WORK ---
/// @param item 
void UVbox::UpdateAlignment(UWidget *item){
    if(item){
        if (UVerticalBoxSlot* Slot = Cast<UVerticalBoxSlot>(item->Slot))
        {
            if(fillHorizontal){
                Slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
                Slot->SetHorizontalAlignment(HAlign_Fill);
            }else{
                Slot->SetHorizontalAlignment(HAlign_Left); // Left, Right, Center, Fill
            }
        }
    }
}

