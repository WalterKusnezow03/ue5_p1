#include "InventorySlot.h"

InventorySlot::InventorySlot(){
    weaponPointer = nullptr;
}

InventorySlot::InventorySlot(Aweapon *in){
    weaponPointer = in;
}

InventorySlot::~InventorySlot(){
    if(weaponPointer != nullptr){
        if (IsValid(weaponPointer)) { // Ensure the weapon is still valid 
            weaponPointer->drop();
        }
        weaponPointer = nullptr;
    }
}

InventorySlot::InventorySlot(const InventorySlot &other){
    if(this != &other){
        *this = other;
    }
}
InventorySlot &InventorySlot::operator=(const InventorySlot &other){
    if(this != &other){
        weaponPointer = other.weaponPointer;
    }
    return *this;
}


bool InventorySlot::IsSamePointer(Aweapon *weapon){
    if(weapon != nullptr && weaponPointer != nullptr){
        return weapon == weaponPointer;
    }
    return false;
}

//slot methods

/// @brief shoots the weapon
void InventorySlot::shoot(){
    if(weaponPointer != nullptr){
        weaponPointer->shoot();
    }
}

/// @brief reload the weapon
/// @param amount 
void InventorySlot::reload(int amount){
    weaponPointer->reload(amount);
}

/// @brief mag size of the weapon
/// @return 
int InventorySlot::getMagSize(){
    if(weaponPointer){
        return weaponPointer->getMagSize();
    }
    return 0;
}

int InventorySlot::getBulletsInMag(){
    if(weaponPointer != nullptr){
        return weaponPointer->getBulletsInMag();
    }
    return 0;
}

/// @brief shows the weapon or not based on passed bool
/// @param show 
void InventorySlot::show(bool show){
    if(weaponPointer != nullptr){
        weaponPointer->showWeapon(show);
    }
}

/// @brief drops the weapon, "this" will be deleted from inventory 
void InventorySlot::drop(){
    if(weaponPointer != nullptr){
        weaponPointer->drop(); //drop the weapon from player
        weaponPointer = nullptr;
    }
}

/// @brief aim or not aim the weapon
/// @param aim 
void InventorySlot::aim(bool aim){
    if(weaponPointer){
        weaponPointer->aim(aim);
    }
}

/// @brief releases left mouse on weapon (holding trigger)
void InventorySlot::releaseShoot(){
    if(weaponPointer){
        weaponPointer->releaseShoot();
    }
}


/// @brief apply recoil value to player just before shooting, will return value based on canshoot
/// @return returns a value != 0 if CAN SHOOT, apply immidiatly
float InventorySlot::recoilValue(){
    if(weaponPointer){
        return weaponPointer->recoilValue(); //recoil value IF CAN SHOOT
    }

    return 0.0f;
}


