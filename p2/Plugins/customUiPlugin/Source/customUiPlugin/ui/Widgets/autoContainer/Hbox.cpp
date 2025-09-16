#include "Hbox.h"
#include "Components/HorizontalBoxSlot.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"


//#include "Components/VerticalBox.h"

void UHbox::init(){
    initHboxIfNeeded();
}

void UHbox::initHboxIfNeeded(){
    if(baseHBox == nullptr){
        baseHBox = NewObject<UHorizontalBox>(this);
    }
}



/// @brief if inited!: adds a item to the hbox and custom visibility changes tracking
/// @param item not nullptr
void UHbox::AddChild(IBaseUiInterface *item){
    initHboxIfNeeded(); // if needed for init() skip fallback (not needed to call after construct)
    if(item && baseHBox){
        UWidget *baseOfItem = item->baseLayoutPointer(); //any widget must exist!
        if(baseOfItem){
            Super::AddChild(item); //track in click listeing and visibilty propagation

            //add to hbox
            //baseHBox->AddChildToHorizontalBox(baseOfItem);
            AddChild(baseOfItem);
        }
    }
}

/// @brief adds any child to the hbox: NOT Custom click listeing, no custom visibility propagation!
/// @param any 
void UHbox::AddChild(UWidget *any){
    initHboxIfNeeded(); // if needed for init() skip fallback (not needed to call after construct)
    if (any && baseHBox){
        baseHBox->AddChildToHorizontalBox(any);
        UpdatePadding(any);
        //UpdateSlotForCustomSlate(any);
    }
}

void UHbox::UpdatePadding(UWidget *widget){
    if(widget){
        if(UHorizontalBoxSlot* Slot = Cast<UHorizontalBoxSlot>(widget->Slot)){
            Slot->SetPadding(makePadding());
        }
    }
}



///@brief removes a child from click listening
void UHbox::RemoveChild(IBaseUiInterface *item){
    if(item){
        Super::RemoveChild(item);
        RemoveChild(item->baseLayoutPointer());
    }
}

///@brief must be overriden
void UHbox::RemoveChild(UWidget *any){
    if(any && baseHBox){
        Super::RemoveChild(any); //tries to find and remove the base interface
        baseHBox->RemoveChild(any);
    }
}


void UHbox::ReplaceChild(int i, IBaseUiInterface *newInterface){
    //Child must be first found from Hbox because UUiComponentParent doesnt
    //have a logical order of items, and uses pop back!
    if (baseHBox && newInterface)
    {
        UWidget *newItem = newInterface->baseLayoutPointer();
        if(newItem != nullptr && i >= 0 && i < baseHBox->GetChildrenCount())
        {
            //replace slot content in place
            UWidget *replaced = baseHBox->GetChildAt(i);
            TArray<UWidget *> afterIndex;
            for (int j = i + 1; j < baseHBox->GetChildrenCount(); j++){
                UWidget *current = baseHBox->GetChildAt(j);
                afterIndex.Add(current);
            }
            //remove from hbox and listening
            RemoveChild(replaced);

            // --- complicated process here because Umg is weird again and shifts widgets :-) ---
            //remove all
            for(UWidget *item : afterIndex){
                baseHBox->RemoveChild(item);
            }
            //add new child in front
            baseHBox->AddChild(newItem);

            //append old childs after new item
            for(UWidget *item : afterIndex){
                baseHBox->AddChild(item);
            }

            //ReplaceAtIndex: Broken
            //Shift: Broken
            //Insert at index: Broken, off by one, no matter what. -1 +1, no fix possible.
            

            return;
        }
        UiDebugHelper::logMessage("HBox could not replace child (widget ptr nullptr)");
    }
    UiDebugHelper::logMessage("HBox could not replace child (box ptr or interface nullptr)");

    /*
    IBaseUiInterface *oldInterface = BaseInterfaceAtIndex(i);
    if(newInterface && oldInterface){



        UWidget *newChild = newInterface->baseLayoutPointer();
        UWidget *oldChild = oldInterface->baseLayoutPointer();
        if (baseHBox && newChild && oldChild)
        {
            int32 Index = baseHBox->GetChildIndex(oldChild);
            if (Index != INDEX_NONE)
            {
                //debug
                UiDebugHelper::logMessage(
                    FString::Printf(
                        TEXT("HBox replace child externalIndex (%d) found(%d)"),
                        i,
                        Index
                    )
                );



                baseHBox->RemoveChildAt(Index);
                baseHBox->InsertChildAt(Index, newChild);
                

                //remove from dispatch and add
                Super::RemoveChild(oldInterface);
                Super::AddChild(newInterface);
            }
        }
        
    }*/
}


/// ---- custom slate integration ----
void UHbox::UpdateSlotForCustomSlate(UWidget *widget){
    if(widget){
        // if derived as custom slate widget set size to be properly drawn.
        UWidgetSlateWrapperBase *slateWidget = Cast<UWidgetSlateWrapperBase>(widget);
        if(slateWidget){
            UHorizontalBoxSlot* Slot = Cast<UHorizontalBoxSlot>(slateWidget->Slot);
            if(Slot){
                Slot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
                Slot->SetPadding(FMargin(5));
                Slot->SetHorizontalAlignment(HAlign_Center);
                Slot->SetVerticalAlignment(VAlign_Center);

                UE_LOG(LogTemp, Display, TEXT("UHbox Slate Widget manipulated"));
            }
        }
    }
}