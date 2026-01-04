#pragma once

#include "customuipluginbase/payload/PayLoadBase.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "WeaponTypePayLoad.generated.h"

/// @brief base class to attach payloads to any ucustomUiComponent
/// derived from uobject for easier mem management.
UCLASS()
class CUSTOMUIPLUGIN_API UWeaponTypePayLoad : public UPayLoadBase{

    GENERATED_BODY()
public:
    void SetType(weaponEnum type){
        typeWeapon = type;
    }

    weaponEnum GetType(){
        return typeWeapon;
    }

private:
    weaponEnum typeWeapon;
};


