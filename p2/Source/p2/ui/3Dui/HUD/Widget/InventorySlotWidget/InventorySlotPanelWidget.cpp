#include "InventorySlotPanelWidget.h"
#include "p2/ui/3Dui/HUD/Widget/InventorySlotWidget/SlotWidget/InventorySlotWidget.h"
#include "customuipluginbase/Dispatcher/Filter/WidgetFilter.h"
#include "p2/_world/PlayerStatManager/Inventory/PlayerStatusInventorySlot.h"


void UInventorySlotPanelWidget::UpdateInventorySlots(TArray<FPlayerStatusInventorySlot> &slotsFromPlayerStatus){
    TArray<UInventorySlotWidget *> childs = GetChilds();
    // update array, hide others if array finished.
    for (int i = 0; i < childs.Num(); i++){
        if(UInventorySlotWidget *current = childs[i]){
            //visible if in bound
            bool visibleIfInBound = i < slotsFromPlayerStatus.Num();
            current->SetVisible(visibleIfInBound);

            //update if in bound
            if(visibleIfInBound){
                FPlayerStatusInventorySlot &slot = slotsFromPlayerStatus[i];
                current->UpdateFrom(slot);
            }
        }
    }
}


TArray<UInventorySlotWidget *> UInventorySlotPanelWidget::GetChilds(){
    TArray<UInventorySlotWidget *> childs;
    if(UWidget *panel = GetPanel()){
        WidgetFilter filter;
        childs = filter.FindDirectChildrenFromPanel<UInventorySlotWidget>(panel);
    }
    return childs;
}