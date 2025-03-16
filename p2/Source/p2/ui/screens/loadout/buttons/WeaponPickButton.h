#pragma once


#include "p2/weapon/weaponEnum.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"

#include "WeaponPickButton.generated.h"

UCLASS()
class P2_API UWeaponPickButton : public UTextButton{

    GENERATED_BODY()

public:

    void setType(weaponEnum typein){
        typeSaved = typein;
    }

    weaponEnum getType(){
        return typeSaved;
    }

private:
    weaponEnum typeSaved;
};