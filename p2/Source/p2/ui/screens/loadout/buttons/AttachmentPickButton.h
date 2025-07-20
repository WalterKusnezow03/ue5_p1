#pragma once



#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/ui/screens/loadout/WeaponContainer.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"

#include "AttachmentPickButton.generated.h"

UCLASS()
class P2_API UAttachmentPickButton : public UTextButton{

    GENERATED_BODY()

public:

    ///@brief will set the type and updat the string automatically from weaponContainer static method
    void setType(weaponAttachmentEnum typein){
        typeSaved = typein;
        updateName();
    }

    weaponAttachmentEnum getType(){
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
                        bindedContainer->updateAnyWeaponAttachment(this->getType());
                        DebugHelper::logMessage("DEBUGCALLBACK picker clicked");
                    } 
                })
            );
        }
    }



private:
    weaponAttachmentEnum typeSaved;

    void updateName(){
        FString name = UWeaponContainer::toString(typeSaved);
        setText(name);
    }
};