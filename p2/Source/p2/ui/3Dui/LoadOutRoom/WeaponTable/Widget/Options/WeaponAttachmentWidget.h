#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"

#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponOptionBaseWidget.h"
#include "p2/weapon/enumUtil/WeaponEnumStringConverter.h"

#include "WeaponAttachmentWidget.generated.h"


UCLASS()
class P2_API UWeaponAttachmentWidget : public UWeaponOptionBaseWidget {
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class USizeBox* baseBox;

    UPROPERTY(EditAnywhere, Category = "WeaponAttachmentType")
    weaponAttachmentEnum type;

    virtual void UpdateTextFromType() override {
        SetText(WeaponEnumStringConverter::toString(type));
    }
};
