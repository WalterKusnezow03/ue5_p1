#include "InventorySlot.h"


InventorySlot::InventorySlot(Aweapon *weaponIn){
    weaponPointer = weaponIn;
}

bool InventorySlot::IsSamePointer(Aweapon *weapon){
    if(weapon != nullptr && weaponPointer != nullptr){
        return weapon == weaponPointer;
    }
    return false;
}


/// @brief apply recoil value to player just before shooting, will return value based on canshoot
/// @return returns a value != 0 if CAN SHOOT, apply immidiatly
float InventorySlot::recoilValue(){
    if(weaponPointer){
        return weaponPointer->recoilValue(); //recoil value IF CAN SHOOT
    }
    return 0.0f;
}


