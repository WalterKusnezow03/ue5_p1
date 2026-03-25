#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventorySlotPanelWidget.generated.h"

class FPlayerStatusInventorySlot;

UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class P2_API UInventorySlotPanelWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void UpdateInventorySlots(TArray<FPlayerStatusInventorySlot> &slotsFromPlayerStatus);

    ///horizontal box where childs are located!
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetPanel();

private:
    TArray<UInventorySlotWidget *> GetChilds();
};