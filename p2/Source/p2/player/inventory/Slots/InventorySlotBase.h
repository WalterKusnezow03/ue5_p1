#pragma once

#include "CoreMinimal.h"


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

    Aweapon *weaponPointer = nullptr;

    bool IsSamePointer(Aweapon *weapon);
    bool IsSameType(Aweapon *weapon);

protected:


};