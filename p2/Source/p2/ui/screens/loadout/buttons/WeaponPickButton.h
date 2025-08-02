#pragma once


#include "AssetPlugin/gameStart/assetEnums/weaponEnum.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/screens/loadout/WeaponContainer.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "p2/weapon/enumUtil/WeaponEnumStringConverter.h"

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

    ///@brief sets the callback for a UWeaponContainer (loadout imte), to this 
    /// button.
    /// --> call this on click of a pick container to
    /// update the binding since the pickables are in n:m relation to the loadout items
    void updateCallbackBind(UWeaponContainer *bindedContainer){
        if(bindedContainer != nullptr){
            SetCallBack(
                FSimpleDelegate::CreateLambda([this, bindedContainer]()
                {
                    if(bindedContainer != nullptr && this){
                        bindedContainer->updateWeaponType(this->getType());
                        DebugHelper::logMessage("DEBUGCALLBACK picker clicked");
                    } 
                })
            );
        }
    }



private:
    weaponEnum typeSaved;

    void updateName(){
        FString name = WeaponEnumStringConverter::toString(typeSaved);
        setText(name);
    }
};