#pragma once

#include "customUiPlugin/ui/_baseClass/payload/PayLoadBase.h"
#include "AssetPlugin/gamestart/assetEnums/weaponEnum.h"
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


