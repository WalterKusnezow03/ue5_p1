#pragma once


#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "p2/ui/screens/gameLaunch/WorldList/WorldList.h"

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


    UFUNCTION()
    void launchWorld(FString world);

protected:


private:

    //layout hbox inside vbox
    UPROPERTY()
    UVbox *menuVbox = nullptr;

    UPROPERTY()
    UHbox *topHbox = nullptr;

    UPROPERTY()
    UWorldList *worldListPicker = nullptr;

    void createTopBar();
    void createMenu();
};