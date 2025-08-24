#pragma once


#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"

#include "GameLaunchScreen.generated.h"

class UPlayerUiBase;
/**
 * loadout screen for the player to choose weapons and attachments
 * 
 */
UCLASS()
class P2_API UGameLaunchScreen : public UCanvasScreen{
    GENERATED_BODY()

public:
    
    virtual void init(UPlayerUiBase &ref);

private:

    //layout hbox inside vbox
    UVbox *menuVbox = nullptr;
    UHbox *baseHbox = nullptr;

    void createBaseLayout();
    void createMenu();
};