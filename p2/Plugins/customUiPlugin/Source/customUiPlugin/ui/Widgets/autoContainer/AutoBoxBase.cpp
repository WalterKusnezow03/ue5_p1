#include "AutoBoxBase.h"


/// @brief marks self and children invisble, also from click dispatch!
/// @param visible 
void UAutoBoxBase::setVisible(bool visible) {
    Super::setVisible(visible); //disables own uwidget base if has one in subtype
    for (int i = 0; i < attachedItems.Num(); i++){
        UcustomUiComponentBase *current = attachedItems[i];
        if (current){
            current->setVisible(visible);
        }
    }
}

/// @brief dispatch click into all children
/// @return any item hit
bool UAutoBoxBase::dispatchClick(){
    bool flag = false;
    if(markedVisible()){ //performance increase
        for (int i = 0; i < attachedItems.Num(); i++){
            UcustomUiComponentBase *current = attachedItems[i];
            if (current){
                if(current->dispatchClick()){
                    flag = true;
                }
            }
        }
    }
    
    return flag;
}

/// @brief ticks all children attached
/// @param deltatime 
void UAutoBoxBase::Tick(float deltatime){
    for (int i = 0; i < attachedItems.Num(); i++){
        UcustomUiComponentBase *current = attachedItems[i];
        if (current){
            current->Tick(deltatime);
        }
    }
}



/// @brief adds a child to click listening and visibility propagation if not null
/// @param item 
void UAutoBoxBase::AddChild(UcustomUiComponentBase *item){
    if(item && CanAddChild(item)){
        attachedItems.Add(item);
    }
}

bool UAutoBoxBase::CanAddChild(UcustomUiComponentBase *ptr){
    return ptr != nullptr && !attachedItems.Contains(ptr);
}

void UAutoBoxBase::RemoveChild(UcustomUiComponentBase *item){
    if(item){
        int32 Index = -1;
        if (attachedItems.Find(item, Index)){
            //swap with back, pop
            int32 lastIndex = attachedItems.Num() - 1;
            attachedItems[Index] = attachedItems[lastIndex];
            attachedItems.Pop();
        }
    }
}



/// -- padding update --
void UAutoBoxBase::SetPadding(FVector2D &paddingIn){
    padding = paddingIn;
    UpdatePaddingForAllTrackedItems();
}

void UAutoBoxBase::UpdatePaddingForAllTrackedItems(){
    for (int i = 0; i < attachedItems.Num(); i++){
        UcustomUiComponentBase *current = attachedItems[i];
        if(current){
            UWidget *layoutPtr = current->baseLayoutPointer();
            if(layoutPtr){
                UpdatePadding(layoutPtr);
            }
            
        }
    }
}

FMargin UAutoBoxBase::makePadding(){
    //l, t, r, b clock wise
    return FMargin(padding.X, padding.Y, padding.X, padding.Y);
}