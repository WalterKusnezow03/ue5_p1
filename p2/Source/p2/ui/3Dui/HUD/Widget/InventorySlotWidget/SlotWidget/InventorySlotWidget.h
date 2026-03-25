#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"

#include "InventorySlotWidget.generated.h"

class FPlayerStatusInventorySlot;

UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class P2_API UInventorySlotWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void SetVisible(bool flag);
    void UpdateFrom(FPlayerStatusInventorySlot &slotToUpdatefrom);

    ///@brief panel where the icons are stored.
    UFUNCTION(BlueprintImplementableEvent, Category = "_WidgetSetting")
    UWidget *GetImagePanel();

    UFUNCTION(BlueprintImplementableEvent, Category = "_WidgetSetting")
    UWidget *GetHighlighter();

protected:
    int ConvertToIndex(weaponEnum payload);
    void ShowIndex(weaponEnum payload);
    void ShowIndex(int indexIn);
    void SetAsHighlightedIndex(bool active);

    UPROPERTY(EditAnywhere, Category = "_WidgetSetting")
    TMap<weaponEnum, int32> TranslationMap;

    int currentIndex = -1;
};