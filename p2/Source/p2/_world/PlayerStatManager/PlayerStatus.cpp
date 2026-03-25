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
            bool isSelectedIndex = i == selectedIndex;
            if(i >= 0 && i < sizeCurrent){
                FPlayerStatusInventorySlot &slotToUpdate = slots[i];
                //returns true if type or selected index flag changed.
                if(slotToUpdate.Update(updateFromPtr, isSelectedIndex)){
                    anySlotChanged = true;
                }
            }else{
                //new slots added.
                anySlotChanged = true; //added new slot.
                slots.Add(FPlayerStatusInventorySlot(updateFromPtr, isSelectedIndex));
                
            }
        }
    }
    return anySlotChanged;
}
