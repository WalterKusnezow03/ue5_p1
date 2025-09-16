#pragma once

#include "customUiPlugin/ui/_baseClass/payload/PayLoadBase.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "WeaponSetupHelperPayload.generated.h"

/// @brief base class to attach payloads to any ucustomUiComponent
/// derived from uobject for easier mem management.
UCLASS()
class CUSTOMUIPLUGIN_API UWeaponSetupHelperPayload : public UPayLoadBase{

    GENERATED_BODY()
public:
    void Set(weaponSetupHelper *ptrIn){
        ptr = ptrIn;
    }

    weaponSetupHelper *Get(){
        return ptr;
    }

private:
    weaponSetupHelper *ptr = nullptr;
};
