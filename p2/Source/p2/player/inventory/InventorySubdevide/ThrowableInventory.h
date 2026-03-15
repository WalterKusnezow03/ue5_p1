#pragma once

#include "p2/player/inventory/InventorySubdevide/InventoryBase.h"
#include "p2/player/inventory/Slots/InventorySlotThrowable.h"


class P2_API ThrowableInventory : public InventoryBase {

public:
    
    virtual void reloadWeapon() override;
    virtual void addToAmmunition(ammunitionEnum type, int count) override;
    virtual TArray<InventorySlotBase *> GetItems() override;

    virtual bool addWeaponIfNotInInventory(Aweapon *weaponIn) override;

    int currentAmmunition();

    virtual void selectIndex(int index) override;

protected:
    TArray<InventorySlotThrowable> slotVector;
    InventorySlotThrowable fallbackThrowableSlot;
    InventorySlotThrowable &CurrentSlotRefThrowable();

    InventorySlotThrowable *FindSlotByType(Aweapon *weapon);
    InventorySlotThrowable *FindSlotByType(weaponEnum typeWeapon);
    InventorySlotThrowable *FindSlotByType(ammunitionEnum typeAmmunition);

    bool IsAThrowableTypeWeapon(Aweapon *weaponIn);
};