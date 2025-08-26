
#pragma once

#include "customUiPLugin/ui/_baseClass/customUiComponentBase.h"

#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "AssetPlugin/gameStart/assetEnums/weaponEnum.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/Widgets/buttons/subtypes/ImageOverlayedButton.h"
#include "customUiPlugin/ui/Widgets/buttons/ButtonBase.h"

#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"

#include "WeaponContainer.generated.h"

class UPlayerUiBase;
/**
 * container to hold spawn information and provide weapoSetup class by value (generation)
 * 
 * holds a UHbox internally: dispatches clicks and visibility automatically!
 * - remember to track in canvasscreen!
 */
UCLASS()
class P2_API UWeaponContainer : public UcustomUiComponentBase{
    GENERATED_BODY()

public:

    //do not use 
    virtual void init() override;
    void init(UPlayerUiBase &refin) override;
    //do not use end

    //use for proper Loadout update on interaction!
    void init(int index, LoadoutHelper &parentLoadout);

    virtual UWidget *baseLayoutPointer(){
        if(baseHorizontalBox){
            return baseHorizontalBox->baseLayoutPointer();
        }
        return nullptr;
        // return baseHorizontalBox;
    }

    // --- CUSTOM CLICK AND VISIBILTY override---
    virtual void setVisible(bool flag) override;
    virtual bool dispatchClick() override;

    //einfachere Api
    void setTextWeapon(FString s);
    void SetCallBackWeapon(FSimpleDelegate callbackIn);

    void setTextSight(FString s);
    void SetCallBackSight(FSimpleDelegate callbackIn);

    void setTextMuzzle(FString s);
    void SetCallBackMuzzle(FSimpleDelegate callbackIn);

    /*
    hier muss intern der waffen typ und attachments gespeichert werden
    um dann andere buttons hier drauf zu binden funktional, das der
    typ aktualisiert wird
    */
    void updateWeaponType(weaponEnum typeIn);
    void updateAnyWeaponAttachment(weaponAttachmentEnum type);

    void updateFrom(weaponSetupHelper &other);

    weaponSetupHelper setupHelperCopy();

    

private:
    UPROPERTY()
    UHbox *baseHorizontalBox = nullptr;

    UPROPERTY()
    UImageOverlayedButton *weaponPreviewImageButton = nullptr;

    UPROPERTY()
    UImageOverlayedButton *attachmentSightImageButton = nullptr;

    UPROPERTY()
    UImageOverlayedButton *attachmentMuzzleImageButton = nullptr;

    void createLayout();
    


    //internal weapon setup object
    weaponSetupHelper setupinternal;

    //loadout helper parent to update
    int ownIndexInLoadoutHelperParent = 0;
    LoadoutHelper *loadoutStorageParent = nullptr;
    void updateLoadoutParent();

    //internal updates
    void updateAllNames();
    void updateWeaponName();
    void updateSightName();
    void updateMuzzleName();
    void updateGripName();

    void updateWeaponSight(weaponAttachmentEnum sightIn);
    void updateWeaponMuzzle(weaponAttachmentEnum typeIn);
    void updateWeaponGrip(weaponAttachmentEnum typein);

    void initUiFromLoadoutParentAtOwnIndex();
};