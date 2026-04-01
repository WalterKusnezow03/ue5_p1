#include "PlayerStatus.h"

#include "p2/player/inventory/PlayerInventory.h"
#include "p2/player/inventory/Slots/InventorySlotBase.h"

FString FPlayerStatus::HealthString(){
    FString outString;
    outString += FString::Printf(TEXT("%d"), health);
    if(maxHealth >= 0){
        outString += FString::Printf(TEXT("|%d"), maxHealth);
    }
    return outString;
}

FString FPlayerStatus::AmmunitionString(){
    FString outString;
    outString += FString::Printf(TEXT("%d"), ammo);
    if(maxAmmo > 0){
        outString += FString::Printf(TEXT("|%d"), maxAmmo);
    }
    return outString;
}



bool FPlayerStatus::UpdateHealth(int number){
    bool changed = number != health;
    health = number;
    return changed;
}

bool FPlayerStatus::UpdateHealthMax(int number){
    bool changed = number != maxHealth;
    maxHealth = number;
    return changed;
}

float FPlayerStatus::healthAsScalar(){
    if(maxHealth != 0){
        float denominator = maxHealth;
        float nominator = health;
        return nominator / denominator;
    }
    return 1.0f;
}

bool FPlayerStatus::UpdateAmmunition(int number){
    bool changed = number != ammo;
    ammo = number;
    return changed;
}

bool FPlayerStatus::UpdateAmmunitionMax(int number){
    bool changed = number != maxAmmo;
    maxAmmo = number;
    return changed;
}



bool FPlayerStatus::UpdatePlayerInventory(PlayerInventory &inventory){
    return UpdatePlayerInventorySlotsChanged(inventory);
}

bool FPlayerStatus::UpdatePlayerInventorySlotsChanged(PlayerInventory &inventory){
    bool anySlotChanged = false;
    int sizeCurrent = slots.Num();
    int selectedIndex = inventory.currentIndexNum();

    TArray<const InventorySlotBase *> inventoryTemp = inventory.GetAllInventorySlots();
    for (int i = 0; i < inventoryTemp.Num(); i++){
        if(const InventorySlotBase* updateFromPtr = inventoryTemp[i]){
            if(UpdateOrAppendPlayerInventorySlot(
                updateFromPtr, 
                i, 
                selectedIndex,
                sizeCurrent
            )){
                anySlotChanged = true;
            }
        }
    }
    if(ShrinkInventory(inventoryTemp.Num())){
        anySlotChanged = true;
    }

    return anySlotChanged;
}

bool FPlayerStatus::UpdateOrAppendPlayerInventorySlot(
    const InventorySlotBase *updateFromPtr,
    int index,
    int selectedIndex,
    int sizeCurrent
)
{
    bool isSelectedIndex = index == selectedIndex;
    if(index >= 0 && index < sizeCurrent){
        FPlayerStatusInventorySlot &slotToUpdate = slots[index];
        //returns true if type or selected index flag changed.
        if(slotToUpdate.Update(updateFromPtr, isSelectedIndex)){
            return true;
        }
    }else{
        //new slots added.
        slots.Add(FPlayerStatusInventorySlot(updateFromPtr, isSelectedIndex));
        return true;
    }
    return false;
}

bool FPlayerStatus::ShrinkInventory(int32 sizeIn){
    if (sizeIn >= 0 && sizeIn <= slots.Num()){
        slots.SetNum(sizeIn);
        return true;
    }
    return false;
}