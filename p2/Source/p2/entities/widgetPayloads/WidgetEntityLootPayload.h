#pragma once

#include "GameCore/PlayerControllerBase/InteractionCallbackInterface/WidgetInteractPayload.h"

#include "p2/weapon/ammunitionEnum.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"


#include "WidgetEntityLootPayload.generated.h"


class Aweapon;

// interaction for widget derived class. 
// returns ammunition to the player.
UCLASS()
class P2_API UWidgetEntityLootPayload : public UWidgetInteractPayload {
    GENERATED_BODY()

public:
    void Setup(Aweapon *weaponPtr); //auto setup by type weapon

    ammunitionEnum GetTypeAmmunition();
    int GetAmmunitionAmount();

protected:
    void Setup(
        int count, 
        weaponEnum typeWeaponIn,
        ammunitionEnum typeAmmunitionIn
    );

    int VerifyAmmunitionCount(int count);

    int ammunition = 1;
    weaponEnum typeWeapon = weaponEnum::assaultRifle;
    ammunitionEnum typeAmmunition;
    // placable item by enum
};