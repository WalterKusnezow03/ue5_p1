#pragma once


#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/weapon/ammunitionEnum.h"
#include <map>

class P2_API WeaponEnumAmmunitionConverter {

public:
    WeaponEnumAmmunitionConverter();
    bool Convert(ammunitionEnum ammo, weaponEnum &typeout);
    bool Convert(weaponEnum weaponType, ammunitionEnum &typeout);

    bool IsThrowable(ammunitionEnum type);

private:
    void SetupMaps();
    void SetupTranslationMap();
    void SetupThrowableAmmunitionTranslationMap();
    std::map<weaponEnum, ammunitionEnum> translationMap;
    std::map<ammunitionEnum, bool> throwableAmmunitionTranslationMap;
};