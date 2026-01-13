#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"




#include "WeaponTableWidget.generated.h"

class AWeaponTableWidgetActor;

UCLASS()
class P2_API UWeaponTableWidget : public UUserWidget {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidget(weaponAttachmentEnum type);

    UFUNCTION(BluePrintCallable, Category = "WidgetSetting")
    void OnClickUpdateSetup(weaponAttachmentEnum type);

    /*
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetSights();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetWeaponSwitches();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetMuzzleAttachments();*/

    void SetWeaponSetupHelperRefernce(weaponSetupHelper *setup);

    void SetParentActor(AWeaponTableWidgetActor *parent);

private:
    weaponSetupHelper *setupHelper = nullptr;
    AWeaponTableWidgetActor *parentActorWidget = nullptr;

    /*
    // -- blueprint api --
    //widget functions to get the check boxes, check selected Mode
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetHealthText();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetAmmunitionText();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidgetTopMessage();
    */
};
