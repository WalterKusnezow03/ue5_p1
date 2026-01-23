#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"

#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponTypeWidget.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponAttachmentWidget.h"


#include "WeaponTableWidget.generated.h"

class AWeaponTableWidgetActor;

UCLASS()
class P2_API UWeaponTableWidget : public UUserWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    //needed ?
    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetWidget(weaponAttachmentEnum type);


    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetPanelSights();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetPanelMuzzleAttachments();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetPanelGripAttachments();


    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetPanelWeaponSwitch();




    void InitWidgets();

    void SetWeaponSetupHelperRefernce(weaponSetupHelper *setup);
    void SetParentActor(AWeaponTableWidgetActor *parent);

    virtual bool dispatchClick(const FVector2D &pos) override;
    virtual bool dispatchHover(const FVector2D &position) override;
    virtual void Tick(float DeltaTime) override {};
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() { return true; }
    virtual UWidget *baseLayoutPointer() { return this; };

private:
    weaponSetupHelper *setupHelper = nullptr;
    AWeaponTableWidgetActor *parentActorWidget = nullptr;

    void OnClickUpdateSetup(weaponAttachmentEnum type);
    void OnClickUpdateSetup(UWeaponAttachmentWidget *widget);

    void OnClickUpdateWeapon(weaponEnum type);
    void OnClickUpdateWeapon(UWeaponTypeWidget *widget);

    bool dispatchClick(const FVector2D &screenPos, UWidget *widget);
    void LogScreenCoordinate(const FVector2D &screenPos);

    //only check one option from list
    void SelectWidgetFromPanel(UWidget *panel, UWidget *found);

    void InitOptionWidgetsFromPanel(UWidget *panel);

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
