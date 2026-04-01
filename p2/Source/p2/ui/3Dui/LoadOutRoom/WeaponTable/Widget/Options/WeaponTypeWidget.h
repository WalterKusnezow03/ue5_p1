#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"

#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponOptionBaseWidget.h"
#include "p2/weapon/enumUtil/WeaponEnumAssetPackProxy.h"

#include "WeaponTypeWidget.generated.h"


UCLASS()
class P2_API UWeaponTypeWidget : public UWeaponOptionBaseWidget {
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class USizeBox* baseBox;

    UPROPERTY(EditAnywhere, Category = "WeaponType")
    weaponEnum type;

    virtual void UpdateTextFromType() override {
        SetText(WeaponEnumAssetPackProxy::toString(type));
    }
};