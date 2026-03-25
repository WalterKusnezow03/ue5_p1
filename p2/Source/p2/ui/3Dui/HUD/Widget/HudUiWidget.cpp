#include "HudUiWidget.h"
#include "Components/TextBlock.h"
#include "p2/ui/3Dui/HUD/Widget/MiniMapWidget/MiniMapWidget.h"
#include "p2/_world/PlayerStatManager/Inventory/PlayerStatusInventorySlot.h"
#include "p2/ui/3Dui/HUD/Widget/InventorySlotWidget/InventorySlotPanelWidget.h"


void UHudUiWidget::UpdateWidget(FPlayerStatus &playerStatus){

    UpdateHealthText(playerStatus.HealthString());
    UpdateAmmunitionText(playerStatus.AmmunitionString());
    UpdateInventorySlots(playerStatus.GetInventorySlots());
}

void UHudUiWidget::UpdateHealthText(FString text){
    UpdateHealthText(FText::FromString(*text));
}

void UHudUiWidget::UpdateHealthText(FText text){
    if(UWidget *widget = GetWidgetHealthText()){
        if(UTextBlock *casted = Cast<UTextBlock>(widget)){
            casted->SetText(text);
        }
    }
}

void UHudUiWidget::UpdateAmmunitionText(FString text){
    UpdateAmmunitionText(FText::FromString(*text));
}

void UHudUiWidget::UpdateAmmunitionText(FText text){
    if(UWidget *widget = GetWidgetAmmunitionText()){
        if(UTextBlock *casted = Cast<UTextBlock>(widget)){
            casted->SetText(text);
        }
    }
}



void UHudUiWidget::UpdateInventorySlots(TArray<FPlayerStatusInventorySlot> &slots){
    if(UWidget *panel = GetInventorySlotPanel()){
        if(UInventorySlotPanelWidget *casted = Cast<UInventorySlotPanelWidget>(panel)){
            casted->UpdateInventorySlots(slots);
        }
    }
}









// ---- mini map dispatch ----

bool UHudUiWidget::InitMiniMap(AActor *player){
    if(UMiniMapWidget *miniMap = MiniMapCasted()){
        miniMap->SetPlayerReference(player);
        return true;
    }
    return false;
}

void UHudUiWidget::Tick(float DeltaTime){
    if(UMiniMapWidget *miniMap = MiniMapCasted()){
        miniMap->Tick(DeltaTime);
    }
}


UMiniMapWidget *UHudUiWidget::MiniMapCasted(){
    if(UWidget *raw = GetMiniMap()){
        if(UMiniMapWidget *casted = Cast<UMiniMapWidget>(raw)){
            return casted;
        }
    }
    return nullptr;
}


// ---- mini map register actor ----
void UHudUiWidget::RegisterActorToMiniMap(AActor *actor, EMarkerType type){
    if(actor){
        if(UMiniMapWidget *casted = MiniMapCasted()){
            casted->AddMarker(type, actor);
        }
    }
}

void UHudUiWidget::UnRegisterActorFromMiniMap(AActor *actor){
    if(actor){
        if(UMiniMapWidget *casted = MiniMapCasted()){
            casted->RemoveMarker(actor);
        }
    }
}