
#pragma once


#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"

#include "customUiPlugin/ui/alignmentPresets/PresetHalfSplitLayout.h"
#include "p2/ui/screens/loadout/buttons/WeaponPickButton.h"
#include "p2/ui/screens/loadout/buttons/AttachmentPickButton.h"
#include "WeaponContainer.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"

#include "LoadoutScreen.generated.h"

class UPlayerUiBase;
/**
 * loadout screen for the player to choose weapons and attachments
 * 
 */
UCLASS()
class P2_API ULoadoutScreen : public UCanvasScreen{
    GENERATED_BODY()

public:
    
    virtual void init(UPlayerUiBase &ref);

    void reloadPlayerLoadoutOnUiLeaveIfNeeded();

private:


    UTextButton *exitButton;
    UPresetHalfSplitLayout *halfSplitBase;

    void createBaseLayout();
    void createHeadline();


    //left side interaction
    void createLeftSideLoadoutMenuItems();
    void openPickerCallBack(int indexChooseType, int containerIndex);

    ///@brief saves all loadout data from the weaponContainers and their updates
    ///which push them to this container on their own because of owning a reference
    ///this object will later be used by the player to update its own inventory 
    LoadoutHelper loadoutinternal;

    //left containers(2 in total)
    std::vector<UWeaponContainer *> weaponContainers; //MUST NOT BE CHANGED AFTER INIT!
    UWeaponContainer *weaponContainer1;
    UWeaponContainer *weaponContainer2;
    bool LoadoutContainersCreated = false;

    void createLoadoutWeaponContainerForValidIndex(int index);

    //right, pickside interaction
    void createRigthListPickers();
    const int WEAPON_PICKER_IDENTIFIER = 0;
    const int SIGHT_PICKER_IDENTIFIER = 1;
    const int MUZZLE_PICKER_IDENTIFIER = 2;

    //PICKER BIND SECTION
    void rebindAllPickers(int indexWeaponPickerFocussed);

    //internal saving of the weapon pick buttons to update callbacks
    std::vector<UWeaponPickButton *> pickableWeaponsButtonVector;

    //internal saving of the sight pick buttons to update callbacks
    std::vector<UAttachmentPickButton *> pickableSightsButtonVector;
    

    //internal saving of the attachment pick buttons to update callbacks
    std::vector<UAttachmentPickButton *> pickableMuzzlesButtonVector;

    
};