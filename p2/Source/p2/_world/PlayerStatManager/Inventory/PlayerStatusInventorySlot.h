#pragma once

#include "CoreMinimal.h"
#include "p2/player/inventory/Slots/InventorySlotBase.h"


class P2_API FPlayerStatusInventorySlot {

public:
    FPlayerStatusInventorySlot();
    ~FPlayerStatusInventorySlot();
    FPlayerStatusInventorySlot(const FPlayerStatusInventorySlot &other);
    FPlayerStatusInventorySlot &operator=(const FPlayerStatusInventorySlot &other);

    FPlayerStatusInventorySlot(const InventorySlotBase *slot, bool active);

    bool Update(const InventorySlotBase *slot, bool active);
    bool Update(const InventorySlotBase *slot);
    bool SetActive(bool active);

    weaponEnum GetType();
    bool IsActiveIndex();

private:
    weaponEnum cachedEnum = weaponEnum::assaultRifle;
    bool isActiveIndex = false;
};