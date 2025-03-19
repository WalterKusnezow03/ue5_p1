
#pragma once

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/alignmentPresets/PresetHalfSplitLayout.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/screens/loadout/buttons/WeaponPickButton.h"
#include "p2/ui/screens/loadout/buttons/SightPickButton.h"
#include "WeaponContainer.h"
#include "Components/HorizontalBox.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"

#include "LoadoutScreen.generated.h"

class UPlayerUi;
/**
 * loadout screen for the player to choose weapons and attachments
 * 
 */
UCLASS()
class P2_API ULoadoutScreen : public UCanvasScreen{
    GENERATED_BODY()

public:
    
    virtual void init(UPlayerUi &ref);

    void reloadPlayerLoadoutOnUiLeaveIfNeeded();

private:


    UTextButton *exitButton;
    UPresetHalfSplitLayout *halfSplitBase;

    void createBaseLayout();
    void createHeadline();


    //left side interaction
    void createLeftSideLoadoutMenuItems();
    void openPickerCallBack(int index, UWeaponContainer *clickComingFrom);

    ///@brief saves all loadout data from the weaponContainers and their updates
    ///which push them to this container on their own because of owning a reference
    ///this object will later be used by the player to update its own inventory 
    LoadoutHelper loadoutinternal;

    //left containers(4 in total)
    std::vector<UWeaponContainer *> weaponContainers; //MUST NOT BE CHANGED AFTER INIT!
    UWeaponContainer *weaponContainer1;
    UWeaponContainer *weaponContainer2;
    bool LoadoutContainersCreated = false;

    void createLoadoutWeaponContainerForValidIndex(int index);

    //right, pickside interaction
    void createRigthListPickers();
    const int WEAPON_PICKER_IDENTIFIER = 0;
    const int SIGHT_PICKER_IDENTIFIER = 1;



    //PICKER BIND SECTION
    void rebindAllPickers(UWeaponContainer *currentWeaponContainerFocussed);

    //internal saving of the weapon pick layout
    UVerticalBox *pickableWeaponsVertical;
    std::vector<UWeaponPickButton *> pickableWeaponsButtonVector;

    //internal saving of the sight pick layout
    UVerticalBox *pickableSightsVertical;
    std::vector<USightPickButton *> pickableSightsButtonVector;
    
    

    
};