
#pragma once

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/alignmentPresets/PresetHalfSplitLayout.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/screens/loadout/buttons/WeaponPickButton.h"
#include "WeaponContainer.h"
#include "Components/HorizontalBox.h"

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



private:
    UTextButton *exitButton;
    UPresetHalfSplitLayout *halfSplitBase;

    void createBaseLayout();
    void createHeadline();


    //left side interaction
    void createRightSideLoadoutMenuItems();
    void openPickerCallBack(int index, UWeaponContainer *clickComingFrom);

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

    //internal saving of the layout
    UVerticalBox *pickableWeaponsVertical;
    std::vector<UWeaponPickButton *> pickableWeaponsButtonVector;
    void rebindAllPickers(UWeaponContainer *currentWeaponContainerFocussed);
    void setCallBackForSelection(
        UWeaponPickButton *buttonFromPickers, // another method for attachments later
        UWeaponContainer *bindedContainer     // may be rebound trought this method!
    );
};