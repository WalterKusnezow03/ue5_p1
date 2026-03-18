#include "WeaponPropertiesMap.h"
#include "WeaponProperties.h"

#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/weapon/ammunitionEnum.h"

//static property
WeaponProperties WeaponPropertiesMap::fallback;
std::map<weaponEnum, WeaponProperties> WeaponPropertiesMap::weaponPropertiesMap;

/// @brief finds a weapon property from the map or a default one.
/// @param typein 
/// @return 
const WeaponProperties &WeaponPropertiesMap::findProperty(weaponEnum typein){
    if(!has(typein)){
        setup(typein);
    }
    if(has(typein)){
        return weaponPropertiesMap[typein];
    }
    return fallback; //should not be reached ever.
}

/// @brief checks if a property is present
/// @param type 
/// @return 
bool WeaponPropertiesMap::has(weaponEnum type){
    return weaponPropertiesMap.find(type) != weaponPropertiesMap.end();
}

/// @brief sets up a weaponProperty
/// @param type 
void WeaponPropertiesMap::setup(weaponEnum type){
    weaponPropertiesMap[type] = WeaponProperties();
    WeaponProperties &refToSetup = weaponPropertiesMap[type]; //created in any case
    int meter = 100;
    /*
    void setup(
        weaponEnum weaponTypeIn, 
        ammunitionEnum ammunitionIn,
        int maxRangeCm, 
        int maxDamageIn,
        float fireRpmIn, //rpm / recoil time
        bool isSingleFireIn,
        int magSize,
        float reloadtime,
        float recoil value
    );
    */
    if (type == weaponEnum::assaultRifle){
        refToSetup.setup(
            type,
            ammunitionEnum::assaultrifle556,
            400 * meter, //max range
            35, //damage max
            400, //rpm / recoil time
            false, //single fire mode
            30, //magSizeIn
            1.2f, //reload time
            0.05f
        );
    }
    if(type == weaponEnum::pistol){
        refToSetup.setup(
            type,
            ammunitionEnum::pistol9,
            400 * meter,
            10, //damage
            300,//rpm / recoil time
            true,
            12,
            1.0f, //reload time
            0.05f
        );
    }

    if(type == weaponEnum::rpgLauncher){
        refToSetup.setup(
            type,
            ammunitionEnum::rpg_ammunition,
            400 * meter,
            10, //damage
            300, //rpm / recoil time
            true, //single fire
            1, //mag size
            2.0f, //reload time
            0.2f //recoil
        );
    }

    /*
    if(type == weaponEnum::thrower){
        refToSetup.setup(
            type,
            ammunitionEnum::grenade_ammunition, //is overriden by throwerweapon class!
            20 * meter, //not needed
            10, //damage
            2, //rpm - throwing stuff is slow.
            true, //single fire
            1, //mag size 1, always relaod.
            1.0f, //reload time
            0.0f //recoil
        );
    }*/
}



/// @brief fast way to find damage and avoid const reference
/// @param typein weapon type
/// @param start
/// @param target 
/// @return damage value from property map
float WeaponPropertiesMap::damageFor(weaponEnum typein, FVector &start, FVector &target){
    const WeaponProperties &ref = findProperty(typein);
    return ref.damageFor(start, target);
}
