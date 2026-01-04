#pragma once

#include "customuipluginbase/payload/PayLoadBase.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "WeaponAttachmentPayLoad.generated.h"

/// @brief base class to attach payloads to any ucustomUiComponent
/// derived from uobject for easier mem management.
UCLASS()
class CUSTOMUIPLUGIN_API UWeaponAttachmentPayLoad : public UPayLoadBase{

    GENERATED_BODY()
public:
    void SetType(weaponAttachmentEnum type){
        typeSaved = type;
    }

    weaponAttachmentEnum GetType(){
        return typeSaved;
    }

private:
    weaponAttachmentEnum typeSaved;
};


