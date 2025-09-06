#include "AutoBoxBase.h"


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