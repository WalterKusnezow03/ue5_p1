#pragma once

#include "CoreMinimal.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include <map>

//forward declare for faster compile
class WeaponProperties;

/**
 * will manage all weaponProperties statically for weapon and ui to acess!
 * dynamic context not needed at all.
 */
class P2_API WeaponPropertiesMap {
public:
    /// @brief NOT Modifiable, read only, Temporary! Use before asking for a new property,
    /// reference might get invalid!
    /// @param typein 
    /// @return 
    static const WeaponProperties &findProperty(weaponEnum typein);

    ///fast find damage
    static float damageFor(weaponEnum typein, FVector &a, FVector &b);

private:
    static bool has(weaponEnum type);
    static void setup(weaponEnum type);

    static WeaponProperties fallback;
    static std::map<weaponEnum, WeaponProperties> weaponPropertiesMap;

    WeaponPropertiesMap(){}
    ~WeaponPropertiesMap(){}

};