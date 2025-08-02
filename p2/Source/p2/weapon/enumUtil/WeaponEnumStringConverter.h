#pragma once 

#include "CoreMinimal.h"

#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "AssetPlugin/gameStart/assetEnums/weaponEnum.h"

class P2_API WeaponEnumStringConverter {

public:
    static FString toString(weaponEnum type);
    static FString toString(weaponAttachmentEnum type);
};