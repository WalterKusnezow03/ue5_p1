
#pragma once


#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"

#include "p2/weapon/setupHelper/LoadoutHelper.h"

#include "p2/ui/screens/loadoutNew/SetupScreen/WeaponSetupScreen.h"

#include "LoadoutScreen.generated.h"

class UPlayerUiBase;
/**
 * loadout screen for the player to choose weapons and attachments
 * 
 */
UCLASS()
class P2_API ULoadoutScreen : public UCanvasScreen{
    GENERATED_BODY()

protected:  
    UPROPERTY()
    UWeaponSetupScreen *weaponSetupScreen = nullptr;

    UPROPERTY()
    UVbox *weaponItemsInventory = nullptr;

    UPROPERTY()
    UVbox *baseVbox = nullptr;

    void Createlayout();
    void FillOrLoadLoadout();
    void RefreshInventoryButtonNames();
    void RefreshButtonByOwnPayLoad(UTextButton *button);
    void OpenWeaponSetupScreen(IBaseUiInterface *buttonClicked);
    void OpenWeaponSetupScreen(weaponSetupHelper *helper);

public:
    /// @brief called when later screen is closed
    virtual void OnReOpenScreen() override; 

    virtual bool dispatchClick(const FVector2D &pos) override;

    virtual void init(UPlayerUiBase &ref);

    void reloadPlayerLoadoutOnUiLeaveIfNeeded();

private:

    ///@brief saves all loadout data from the weaponContainers and their updates
    ///which push them to this container on their own because of owning a reference
    ///this object will later be used by the player to update its own inventory 
    LoadoutHelper loadoutinternal;


    
};