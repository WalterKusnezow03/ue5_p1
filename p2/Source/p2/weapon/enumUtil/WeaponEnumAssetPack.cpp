#include "WeaponEnumAssetPack.h"


void UWeaponEnumAssetPack::ValidatePropertiesOnInit(){
    for (TPair<weaponEnum, FWeaponPropertyAsset>& Pair : conversionAssets){
        FWeaponPropertyAsset &ValueRef = Pair.Value;
        ValueRef.ValidatePropertiesOnInit();
    }
}

bool UWeaponEnumAssetPack::CanHaveAttachments(weaponEnum typeWeapon){
    if (FWeaponPropertyAsset* value = FindByWeapon(typeWeapon)){
        return value->CanHaveAttachments;
    }
    return false;
}

bool UWeaponEnumAssetPack::isASightAttachment(weaponAttachmentEnum type){
    return HasType<weaponAttachmentEnum>(type, sightAttachments);
}

bool UWeaponEnumAssetPack::isAMuzzleAttachment(weaponAttachmentEnum type){
    return HasType<weaponAttachmentEnum>(type, muzzleAttachments);
}

bool UWeaponEnumAssetPack::isAGripAttachment(weaponAttachmentEnum type){
    return HasType<weaponAttachmentEnum>(type, gripAttachments);
}

FString UWeaponEnumAssetPack::toString(weaponEnum typeWeapon){
    if (FWeaponPropertyAsset* value = FindByWeapon(typeWeapon)){
        FString outname;
        if (value->GetUiName(outname)){
            return outname;
        }
    }
    return TEXT("NONE");
}

FString UWeaponEnumAssetPack::toString(weaponAttachmentEnum type){
    if(FString *s = attachmentNames.Find(type)){
        if(s->Len() > 0){
            return *s;
        }
    }
    return TEXT("NONE");
}


//find by weapon enum
FWeaponPropertyAsset* UWeaponEnumAssetPack::FindByWeapon(weaponEnum type){
    if (FWeaponPropertyAsset* value = conversionAssets.Find(type)){
        return value;
    }
    return nullptr;
}

// find by ammunition
FWeaponPropertyAsset* UWeaponEnumAssetPack::FindByAmmunition(ammunitionEnum ammoType){
    for (TPair<weaponEnum, FWeaponPropertyAsset>& Pair : conversionAssets){
        FWeaponPropertyAsset &ValueRef = Pair.Value;
        if(ValueRef.IsSameAmmunitionAndThrowable(ammoType)){
            return &ValueRef;
        }
    }
    return nullptr;
}





bool UWeaponEnumAssetPack::Convert(ammunitionEnum ammo, weaponEnum &typeout){
    //key needed here, cant find by ammunition raw.
    for (TPair<weaponEnum, FWeaponPropertyAsset>& Pair : conversionAssets){
        weaponEnum Key = Pair.Key;
        FWeaponPropertyAsset &ValueRef = Pair.Value;
        if(ValueRef.IsSameAmmunition(ammo)){
            typeout = Key;
            return true;
        }
    }
    return false;
}

bool UWeaponEnumAssetPack::Convert(weaponEnum weaponType, ammunitionEnum &typeout){
    if(FWeaponPropertyAsset* found = FindByWeapon(weaponType)){
        typeout = found->typeAmmunition;
        return true;
    }
    return false;
}

bool UWeaponEnumAssetPack::IsThrowable(ammunitionEnum ammoType){
    if(FWeaponPropertyAsset *found = FindByAmmunition(ammoType)){
        return found->IsThrowable();
    }
    return false;
}






int UWeaponEnumAssetPack::getMagSize(weaponEnum weaponType){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->getMagSize();
    }
    return 0;
}

float UWeaponEnumAssetPack::recoilValue(weaponEnum weaponType){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->recoilValue();
    }
    return 0.0f;
}

float UWeaponEnumAssetPack::reloadTime(weaponEnum weaponType){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->reloadTime();
    }
    return 0.0f;
}

/// @brief damage over a distance by weapon type
float UWeaponEnumAssetPack::damageFor(weaponEnum weaponType, FVector &a, FVector &b){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->damageFor(a,b);
    }
    return 0.0f;
}

float UWeaponEnumAssetPack::CooldownTimeBasedOnRpm(weaponEnum weaponType){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->CooldownTimeBasedOnRpm();
    }
    return 0.0f;
}

ammunitionEnum UWeaponEnumAssetPack::getAmmunitionType(weaponEnum weaponType){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->typeAmmunition;
    }
    return ammunitionEnum::assaultrifle556;
}

bool UWeaponEnumAssetPack::isSingleFireOnly(weaponEnum weaponType){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->isSingleFireOnly();
    }
    return false;
}


EMagSocketType UWeaponEnumAssetPack::getMagSocketType(weaponEnum weaponType){
    if (FWeaponPropertyAsset* value = FindByWeapon(weaponType)){
        return value->typeMagSocket;
    }
    return EMagSocketType::UTIL_MAG;
}
    