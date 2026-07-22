#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/ammunitionEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/PlateCarrier/MagSocket/MagSocketType.h"

class UWeaponEnumAssetPack;

/// @brief proxy for UWeaponEnumAssetPack, conversion of types, strings and all other
/// properties of interest.
class P2_API WeaponEnumAssetPackProxy
{
    
public:
    static void ValidatePropertiesOnInit();

    static bool CanHaveAttachments(weaponEnum typeWeapon);
    static bool isASightAttachment(weaponAttachmentEnum type);
    static bool isAMuzzleAttachment(weaponAttachmentEnum type);
    static bool isAGripAttachment(weaponAttachmentEnum type);

    static FString toString(weaponEnum type);
    static FString toString(weaponAttachmentEnum type);

    ///needed for explicit throwable weapon to ammunition conversion, not
    ///rifles / pistols! baseball bat, grenade, etc.! (1/1) map, not (1/n)!
    static bool Convert(ammunitionEnum ammo, weaponEnum &typeout);
    static bool Convert(weaponEnum weaponType, ammunitionEnum &typeout);
    static bool IsThrowable(ammunitionEnum type);

    static int getMagSize(weaponEnum weaponType);
    static float recoilValue(weaponEnum weaponType);
    static float reloadTime(weaponEnum weaponType);

    /// @brief damage over a distance by weapon type
    static float damageFor(weaponEnum weaponType, FVector &a, FVector &b);
    static float CooldownTimeBasedOnRpm(weaponEnum weaponType);
    static ammunitionEnum getAmmunitionType(weaponEnum weaponType);
    static bool isSingleFireOnly(weaponEnum weaponType);

    static EMagSocketType getMagSocketType(weaponEnum weaponType);

private:
    static UWeaponEnumAssetPack *FindInstance();
};