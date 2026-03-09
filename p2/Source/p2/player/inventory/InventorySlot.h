#pragma once

#include "CoreMinimal.h"

#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"

class P2_API InventorySlot {

public:
    InventorySlot();
    InventorySlot(Aweapon *in);
    ~InventorySlot();

    InventorySlot(const InventorySlot &other);
    InventorySlot &operator=(const InventorySlot &other);

    Aweapon *weaponPointer = nullptr;
    
    void shoot();
    void reload(int amount);
    void drop();
    void show(bool show);
    void aim(bool aim);
    void releaseShoot();
    int getMagSize();
    int getBulletsInMag();
    float recoilValue();

    bool IsSamePointer(Aweapon *weapon);
};