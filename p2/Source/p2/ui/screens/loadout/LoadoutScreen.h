
#pragma once

#include "p2/ui/screens/CanvasScreen.h"
#include "p2/ui/alignmentPresets/PresetHalfSplitLayout.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"

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
    void createSideMenu();





    void debugAddSomeItems();

    UTextButton *firstSlotButton;
};