#pragma once 

#include "CoreMinimal.h"

#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"

class P2_API WeaponEnumStringConverter {

public:
    static FString toString(weaponEnum type);
    static FString toString(weaponAttachmentEnum type);

    static std::vector<weaponEnum> AllWeaponTypes();
};