#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"

#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponTypeWidget.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponAttachmentWidget.h"


#include "ExitButtonWidget.generated.h"

class ALoadoutRoomExitButtonActor;

UCLASS()
class P2_API UExitButtonWidget : public UUserWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    
    void SetParentActor(ALoadoutRoomExitButtonActor *parent);

    virtual bool dispatchClick(const FVector2D &pos) override;
    virtual void Tick(float DeltaTime) override {};
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() { return true; }
    virtual UWidget *baseLayoutPointer() { return this; };

private:
   
    ALoadoutRoomExitButtonActor *parentActorWidget = nullptr;

};
