#pragma once


#include "p2/weapon/weaponEnum.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/screens/loadout/WeaponContainer.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"

#include "WeaponPickButton.generated.h"

UCLASS()
class P2_API UWeaponPickButton : public UTextButton{

    GENERATED_BODY()

public:

    ///@brief will set the type and updat the string automatically from weaponContainer static method
    void setType(weaponEnum typein){
        typeSaved = typein;
        updateName();
    }

    weaponEnum getType(){
        return typeSaved;
    }

private:
    weaponEnum typeSaved;

    void updateName(){
        FString name = UWeaponContainer::toString(typeSaved);
        setText(name);
    }
};