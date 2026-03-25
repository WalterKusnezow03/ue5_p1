#pragma once

#include "CoreMinimal.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/weapon/weapon.h"

class P2_API InventorySlotBase {

public:
    void shoot();
    
    
    virtual float recoilValue(){
        return 0.0f;
    }
    void show(bool show);
    void drop();

    virtual void aim(bool flag);
    virtual void releaseShoot();
    virtual void reload(int amount);

    virtual int getMagSize();
    virtual int getBulletsInMag();

    Aweapon *GetWeaponPointer();
    void ResetWeaponPointer();
    void SetWeaponPointer(Aweapon *weaponIn);

    bool IsSamePointer(Aweapon *weapon);
    bool IsSameType(Aweapon *weapon);

    //may be overriden
    virtual bool GetWeaponType(weaponEnum &typeOut) const;

protected:

    Aweapon *weaponPointer = nullptr;

};