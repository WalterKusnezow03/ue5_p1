#pragma once

#include "CoreMinimal.h"
#include "p2/_world/PlayerStatManager/Inventory/PlayerStatusInventorySlot.h"

class PlayerInventory;
class InventorySlotBase;

/// @brief struct storing player info for HUD
class P2_API FPlayerStatus {

public:
    FPlayerStatus(){};
    ~FPlayerStatus(){};

private:
    int health = 0;
    int maxHealth = 0;

    int ammo = 0;
    int maxAmmo = 0;

    int mags = 0;

    int selectedSlotIndex = -1;
    TArray<FPlayerStatusInventorySlot> slots;

public:
    ///@brief update and return if anything changed
    bool UpdateHealth(int number);
    bool UpdateHealthMax(int number);
    bool UpdateAmmunition(int number);
    bool UpdateAmmunitionMax(int number);
    bool UpdatePlayerInventory(PlayerInventory &inventory);

    //api for HUD
    FString HealthString();
    FString AmmunitionString();
    TArray<FPlayerStatusInventorySlot> &GetInventorySlots(){
        return slots;
    }

    /// @brief value between [0,1] if maxhealth is a valid number.
    float healthAsScalar();

private:
    bool UpdatePlayerInventorySlotsChanged(PlayerInventory &inventory);
    bool UpdateOrAppendPlayerInventorySlot(
        const InventorySlotBase *updateFromPtr,
        int index,
        int selectedIndex,
        int sizeCurrent
    );
    bool ShrinkInventory(int32 sizeIn);
};