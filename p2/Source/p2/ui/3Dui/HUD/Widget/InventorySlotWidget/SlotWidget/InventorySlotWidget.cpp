#include "InventorySlotWidget.h"
#include "p2/_world/PlayerStatManager/Inventory/PlayerStatusInventorySlot.h"
#include "customuipluginbase/baseInterface/WidgetHelper.h"
#include "customuipluginbase/Dispatcher/Filter/WidgetFilter.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/weapon/enumUtil/WeaponEnumAssetPackProxy.h"

void UInventorySlotWidget::SetVisible(bool flag){
    WidgetHelper::SetVisible(this, flag);
}

void UInventorySlotWidget::UpdateFrom(FPlayerStatusInventorySlot &slotToUpdatefrom){
    ShowIndex(slotToUpdatefrom.GetType());
    SetAsHighlightedIndex(slotToUpdatefrom.IsActiveIndex());
}

void UInventorySlotWidget::SetAsHighlightedIndex(bool active){
    if(UWidget *widget = GetHighlighter()){
        WidgetHelper::SetVisible(widget, active);
    }
}




int UInventorySlotWidget::ConvertToIndex(weaponEnum payload){
    if (int32* Value = TranslationMap.Find(payload))
    {
        int32 Result = *Value;
        return Result;
    }
    return -1;
}

void UInventorySlotWidget::ShowIndex(weaponEnum payload){
    LogIndex(payload);
    ShowIndex(ConvertToIndex(payload));
}

void UInventorySlotWidget::LogIndex(weaponEnum payload){
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("UInventorySlotWidget::Index %d %s"),
            ConvertToIndex(payload),
            *WeaponEnumAssetPackProxy::toString(payload)
        )
    );
}




void UInventorySlotWidget::ShowIndex(int indexIn){
    if(currentIndex != indexIn){
        currentIndex = indexIn;
        
        if(UWidget *panel = GetImagePanel()){
            WidgetFilter filter;
            TArray<UWidget*> childs = filter.FindDirectChildrenFromPanel<UWidget>(panel);
            for (int i = 0; i < childs.Num(); i++){
                if(UWidget *current = childs[i]){
                    bool visible = i == currentIndex;
                    WidgetHelper::SetVisible(current, visible);
                }
            }
        }
    }
}