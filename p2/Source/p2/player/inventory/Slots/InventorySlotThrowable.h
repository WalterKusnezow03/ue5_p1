#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "p2/player/inventory/Slots/InventorySlotBase.h"
#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"



/// @brief different behaiviour for throwable weapon items
class P2_API InventorySlotThrowable : public InventorySlotBase {

public:
    InventorySlotThrowable() {};
    InventorySlotThrowable(Aweapon *weaponIn, UCameraComponent *cameraIn);
    InventorySlotThrowable(weaponEnum typeEnum, UCameraComponent *cameraIn);

    void Setup(UCameraComponent *camera);

    virtual void reload(int amount) override;
    virtual void releaseShoot() override;
    
    virtual int getMagSize() override;
    virtual int getBulletsInMag() override;
    virtual float recoilValue() override;

    bool IsSameType(Aweapon *weapon);
    bool IsSameType(weaponEnum type);
    bool IsSameType(ammunitionEnum type);
    int AmmunitionInnerCount(){
        return innerAmmunition;
    }

    virtual bool GetWeaponType(weaponEnum &outType) const override;

private: 
    int innerAmmunition = 4;
    weaponEnum typeThrowable = weaponEnum::grenade;
    void TryReloadOnReleaseShoot();
    void replaceInstance();

    UCameraComponent *camera = nullptr;
};