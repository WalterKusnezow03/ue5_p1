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



void UVbox::ReplaceChild(int i, IBaseUiInterface *newInterface){
    IBaseUiInterface *oldInterface = BaseInterfaceAtIndex(i);
    if(newInterface && oldInterface){

        UWidget *newChild = newInterface->baseLayoutPointer();
        UWidget *oldChild = oldInterface->baseLayoutPointer();
        if (baseVBox && newChild && oldChild)
        {
            int32 Index = baseVBox->GetChildIndex(oldChild);
            if (Index != INDEX_NONE)
            {
                baseVBox->RemoveChildAt(Index);
                baseVBox->InsertChildAt(Index, newChild);

                //remove from dispatch and add
                Super::RemoveChild(oldInterface);
                Super::AddChild(newInterface);
            }
        }
        
    }
}






void UVbox::UpdatePadding(UWidget *widget){
    if(widget){
        if(UVerticalBoxSlot* Slot = Cast<UVerticalBoxSlot>(widget->Slot)){
            Slot->SetPadding(
                //Super::
                makePadding()
            );
        }
    }
}



// -- Alignment vbox exclusive --

void UVbox::SetItemsFillHorizontal(){
    fillHorizontal = true;
    UpdateAlignmentForAllTrackedItems();
}

//unclear if this even works.
void UVbox::UpdateAlignmentForAllTrackedItems(){
    for (int i = 0; i < trackedChildsInterface.Num(); i++){
        IBaseUiInterface *current = trackedChildsInterface[i];
        if(current){
            UWidget *layoutPtr = current->baseLayoutPointer();
            if(layoutPtr){
                UpdateAlignment(layoutPtr);//unclear if this even works.
            }
        }
    }
}

/// @brief --- DOES NOT WORK --- //unclear if this even works.
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



#include "customUiPlugin/ui/Widgets/autoContainer/sizing/FixedSizeBox.h"
void UVbox::CreateSpacer(int height){
    height = std::max(height, 0);

    UFixedSizeBox *box = NewWidgetInitialized<UFixedSizeBox>(this);
    box->SetWidth(height);
    box->SetHeight(height);

    UVerticalBoxSlot *SpacerSlot = baseVBox->AddChildToVerticalBox(box->baseLayoutPointer());
    if (SpacerSlot)
    {
        SpacerSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); // füllt restlichen Platz
    }

}
