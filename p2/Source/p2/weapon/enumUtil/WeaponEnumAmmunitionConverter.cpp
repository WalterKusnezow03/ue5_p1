#include "WeaponEnumAmmunitionConverter.h"

WeaponEnumAmmunitionConverter::WeaponEnumAmmunitionConverter(){
    SetupMaps();
}


bool WeaponEnumAmmunitionConverter::Convert(weaponEnum weaponType, ammunitionEnum &typeOut){
    if(translationMap.find(weaponType) != translationMap.end()){
        ammunitionEnum copy = translationMap[weaponType];
        typeOut = copy;
        return true;
    }
    return false;
}

bool WeaponEnumAmmunitionConverter::Convert(ammunitionEnum ammoTargeted, weaponEnum &typeOut){
    for(auto &pair : translationMap){
        ammunitionEnum currentAmmoType = pair.second;
        if(currentAmmoType == ammoTargeted){
            weaponEnum copy = pair.first;
            typeOut = copy;
            return true;
        }
    }
    return false;
}

void WeaponEnumAmmunitionConverter::SetupMaps(){
    SetupTranslationMap();
    SetupThrowableAmmunitionTranslationMap();
}

void WeaponEnumAmmunitionConverter::SetupTranslationMap(){
    translationMap[weaponEnum::assaultRifle] = ammunitionEnum::assaultrifle556;
    translationMap[weaponEnum::pistol] = ammunitionEnum::pistol9;
    //translationMap[weaponEnum::thrower] = ammunitionEnum::; --- todo ---
    translationMap[weaponEnum::C4] = ammunitionEnum::c4_ammunition;
    translationMap[weaponEnum::grenade] = ammunitionEnum::grenade_ammunition;
    translationMap[weaponEnum::smokeGrenade] = ammunitionEnum::smokegrenade_ammunition;
}

void WeaponEnumAmmunitionConverter::SetupThrowableAmmunitionTranslationMap(){
    throwableAmmunitionTranslationMap[ammunitionEnum::grenade_ammunition] = true;
    throwableAmmunitionTranslationMap[ammunitionEnum::molotov_ammunition] = true;
    throwableAmmunitionTranslationMap[ammunitionEnum::smokegrenade_ammunition] = true;
}


bool WeaponEnumAmmunitionConverter::IsThrowable(ammunitionEnum type){
    if(throwableAmmunitionTranslationMap.find(type) != throwableAmmunitionTranslationMap.end()){
        return true;
    }
    return false;
}