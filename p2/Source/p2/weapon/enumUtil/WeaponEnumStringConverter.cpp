#include "WeaponEnumStringConverter.h"

FString WeaponEnumStringConverter::toString(weaponEnum type){
    FString name = FString::Printf(TEXT("<weapon name>"));
    if (type == weaponEnum::assaultRifle){
        name = FString::Printf(TEXT("assault rifle"));
    }
    if(type == weaponEnum::pistol){
        name = FString::Printf(TEXT("pistol"));
    }
    return name;
}

std::vector<weaponEnum> WeaponEnumStringConverter::AllWeaponTypes(){
    std::vector<weaponEnum> output = {
        weaponEnum::assaultRifle,
        weaponEnum::pistol,
        weaponEnum::thrower
    };
    return output;
}

FString WeaponEnumStringConverter::toString(weaponAttachmentEnum type){
    FString name = FString::Printf(TEXT("<attachment name>"));
    
    if (type == weaponAttachmentEnum::iron_sight){
        name = FString::Printf(TEXT("iron sight"));
    }
    if (type == weaponAttachmentEnum::reddot){
        name = FString::Printf(TEXT("reddot sight"));
    }
    if (type == weaponAttachmentEnum::grip_vertical){
        name = FString::Printf(TEXT("grip vertical"));
    }
    if(type == weaponAttachmentEnum::muzzle_flashSurpressor){
        name = FString::Printf(TEXT("flash surpressor"));
    }
    if(type == weaponAttachmentEnum::muzzle_SoundSurpressor){
        name = FString::Printf(TEXT("sound surpressor"));
    }
    
    return name;
} 
