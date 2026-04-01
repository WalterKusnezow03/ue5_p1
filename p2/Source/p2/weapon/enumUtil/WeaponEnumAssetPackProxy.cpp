#include "WeaponEnumAssetPackProxy.h"
#include "p2/weapon/enumUtil/WeaponEnumAssetPack.h"
#include "AssetEnumCollection/assetEnums/EDataAssetEnum.h"
#include "AssetPlugin/gamestart/assetManager.h"

void WeaponEnumAssetPackProxy::ValidatePropertiesOnInit(){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->ValidatePropertiesOnInit();
    }
}

UWeaponEnumAssetPack *WeaponEnumAssetPackProxy::FindInstance(){
    if(assetManager *a = assetManager::instance()){
        if(UWeaponEnumAssetPack* pack = a->Find<EDataAssetEnum, UWeaponEnumAssetPack>(
            EDataAssetEnum::WeaponDataAssetPack
        )){
            return pack;
        }
    }
    return nullptr;
}



bool WeaponEnumAssetPackProxy::CanHaveAttachments(weaponEnum typeWeapon){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->CanHaveAttachments(typeWeapon);
    }
    return false;
}


bool WeaponEnumAssetPackProxy::isASightAttachment(weaponAttachmentEnum type){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->isASightAttachment(type);
    }
    return false;
}
bool WeaponEnumAssetPackProxy::isAMuzzleAttachment(weaponAttachmentEnum type){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->isAMuzzleAttachment(type);
    }
    return false;
}

bool WeaponEnumAssetPackProxy::isAGripAttachment(weaponAttachmentEnum type){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->isAGripAttachment(type);
    }
    return false;
}

FString WeaponEnumAssetPackProxy::toString(weaponEnum type){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->toString(type);
    }
    return TEXT("NONE");
}

FString WeaponEnumAssetPackProxy::toString(weaponAttachmentEnum type){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->toString(type);
    }
    return TEXT("NONE");
}


///needed for explicit throwable weapon to ammunition conversion, not
///rifles / pistols! baseball bat, grenade, etc.! (1/1) map, not (1/n)!
bool WeaponEnumAssetPackProxy::Convert(ammunitionEnum ammo, weaponEnum &typeout){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->Convert(ammo, typeout);
    }
    return false;
}


bool WeaponEnumAssetPackProxy::Convert(weaponEnum weaponType, ammunitionEnum &typeout){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->Convert(weaponType, typeout);
    }
    return false;
}

bool WeaponEnumAssetPackProxy::IsThrowable(ammunitionEnum type){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->IsThrowable(type);
    }
    return false;
}


int WeaponEnumAssetPackProxy::getMagSize(weaponEnum weaponType){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->getMagSize(weaponType);
    }
    return 0;
}

float WeaponEnumAssetPackProxy::recoilValue(weaponEnum weaponType){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->recoilValue(weaponType);
    }
    return 0.0f;
}

float WeaponEnumAssetPackProxy::reloadTime(weaponEnum weaponType){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->reloadTime(weaponType);
    }
    return 0.0f;
}

/// @brief damage over a distance by weapon type
float WeaponEnumAssetPackProxy::damageFor(weaponEnum weaponType, FVector &a, FVector &b){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        DebugHelper::logMessage("WeaponEnumAssetPackProxy::DAMAGE");
        return pack->damageFor(weaponType, a, b);
    }
    return 0.0f;
}

float WeaponEnumAssetPackProxy::CooldownTimeBasedOnRpm(weaponEnum weaponType){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->CooldownTimeBasedOnRpm(weaponType);
    }
    return 0.0f;
}

ammunitionEnum WeaponEnumAssetPackProxy::getAmmunitionType(weaponEnum weaponType){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->getAmmunitionType(weaponType);
    }
    return ammunitionEnum::assaultrifle556;
}

bool WeaponEnumAssetPackProxy::isSingleFireOnly(weaponEnum weaponType){
    if(UWeaponEnumAssetPack *pack = FindInstance()){
        return pack->isSingleFireOnly(weaponType);
    }
    return false;
}
