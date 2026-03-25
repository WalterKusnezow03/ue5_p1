#include "PlayerStatusInventorySlot.h"


FPlayerStatusInventorySlot::FPlayerStatusInventorySlot(){

}

FPlayerStatusInventorySlot::~FPlayerStatusInventorySlot(){

}

FPlayerStatusInventorySlot::FPlayerStatusInventorySlot(const FPlayerStatusInventorySlot &other){
    if(this != &other){
        *this = other;
    }
}
FPlayerStatusInventorySlot &FPlayerStatusInventorySlot::operator=(const FPlayerStatusInventorySlot &other){
    if(this != &other){
        cachedEnum = other.cachedEnum;
    }
    return *this;
}

FPlayerStatusInventorySlot::FPlayerStatusInventorySlot(const InventorySlotBase *slot, bool active){
    Update(slot, active);
}

bool FPlayerStatusInventorySlot::Update(const InventorySlotBase *slot, bool active){
    bool resultA = SetActive(active);
    bool resultB = Update(slot);
    return resultA || resultB;
}

bool FPlayerStatusInventorySlot::Update(const InventorySlotBase *slot){
    if(slot){
        weaponEnum newValue = cachedEnum;
        if(slot->GetWeaponType(newValue)){
            if(newValue != cachedEnum){
                cachedEnum = newValue;
                return true;
            }
        }
    }
    return false;
}

weaponEnum FPlayerStatusInventorySlot::GetType(){
    return cachedEnum;
}

bool FPlayerStatusInventorySlot::IsActiveIndex(){
    return isActiveIndex;
}


bool FPlayerStatusInventorySlot::SetActive(bool active){
    bool changed = isActiveIndex != active;
    isActiveIndex = active;
    return changed;
}