#pragma once

#include "CoreMinimal.h"

#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"
#include "InventorySlotBase.h"

class P2_API InventorySlot : public InventorySlotBase {

public:
    InventorySlot() {};
    InventorySlot(Aweapon *weaponIn);
    virtual float recoilValue() override;
    bool IsSamePointer(Aweapon *weapon);

    
};