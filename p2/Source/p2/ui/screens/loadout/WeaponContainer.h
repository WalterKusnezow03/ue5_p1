
#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/HorizontalBox.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"
#include "WeaponContainer.generated.h"

/**
 * container to hold spawn information and provide weapoSetup class by value (generation)
 */
UCLASS()
class UWeaponContainer : public UcustomUiComponentBase{
    GENERATED_BODY()

public:
    virtual void init() override;

    virtual UWidget *baseLayoutPointer(){
        return baseHorizontalBox;
    }

    //einfachere Api
    void setTextWeapon(FString s);
    void SetCallBackWeapon(FSimpleDelegate callbackIn);

    void setTextSight(FString s);
    void SetCallBackSight(FSimpleDelegate callbackIn);

    /*
    hier muss intern der waffen typ und attachments gespeichert werden
    um dann andere buttons hier drauf zu binden funktional, das der
    typ aktualisiert wird
    */
    void updateWeaponType(weaponEnum typeIn);
    void updateWeaponSight(weaponAttachmentEnum sightIn);

    weaponSetupHelper setupHelperCopy(){
        return setupinternal;
    }

private:
    UHorizontalBox *baseHorizontalBox;

    UImageOverlayedButton *weaponPreviewImageButton;
    UImageOverlayedButton *attachmentSightImageButton;

    void createLayout();
    void addToHorizontalBox(UcustomUiComponentBase *any);


    //internal weapon setup object
    weaponSetupHelper setupinternal;


    //internal updates
    void updateWeaponName();
};