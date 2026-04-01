#include "InventorySlotBase.h"
#include "p2/weapon/enumUtil/WeaponEnumAssetPackProxy.h"

bool InventorySlotBase::IsSamePointer(Aweapon *weapon){
    if(weapon != nullptr && weaponPointer != nullptr){
        return weapon == weaponPointer;
    }
    return false;
}

bool InventorySlotBase::IsSameType(Aweapon *weapon){
    if(weapon != nullptr && weaponPointer != nullptr){
        return weapon == weaponPointer;
    }
    return false;
}

/// @brief shoots the weapon
void InventorySlotBase::shoot(){
    if(weaponPointer != nullptr){
        weaponPointer->shoot();
    }
}


/// @brief shows the weapon or not based on passed bool
/// @param show 
void InventorySlotBase::show(bool show){
    if(weaponPointer != nullptr){
        weaponPointer->showWeapon(show);
    }
}

/// @brief drops the weapon, "this" will be deleted from inventory 
void InventorySlotBase::drop(){
    if(weaponPointer != nullptr){
        weaponPointer->drop(); //drop the weapon from player
        weaponPointer = nullptr;
    }
}

/// @brief aim or not aim the weapon
/// @param aim 
void InventorySlotBase::aim(bool aim){
    if(weaponPointer){
        weaponPointer->aim(aim);
    }
}


void InventorySlotBase::releaseShoot(){
    if(weaponPointer){
        weaponPointer->releaseShoot();
    }
}


/// @brief mag size of the weapon
/// @return 
int InventorySlotBase::getMagSize(){
    if(weaponPointer){
        return weaponPointer->getMagSize();
    }
    return 0;
}

int InventorySlotBase::getBulletsInMag(){
    if(weaponPointer != nullptr){
        return weaponPointer->getBulletsInMag();
    }
    return 0;
}


/// @brief reload the weapon
/// @param amount 
void InventorySlotBase::reload(int amount){
    weaponPointer->reload(amount);
}



Aweapon *InventorySlotBase::GetWeaponPointer() {
    return weaponPointer;
}

void InventorySlotBase::ResetWeaponPointer(){
    weaponPointer = nullptr;
}

void InventorySlotBase::SetWeaponPointer(Aweapon *weaponIn){
    weaponPointer = weaponIn;
}


bool InventorySlotBase::GetWeaponType(weaponEnum &outType) const {
    if(weaponPointer){
        outType = weaponPointer->weaponType();
        return true;
    }
    return false;
}


void InventorySlotBase::AppendToString(FString &outMessage) const {
    weaponEnum type;
    if (GetWeaponType(type)){
        FString weaponString = WeaponEnumAssetPackProxy::toString(type);
        outMessage += FString::Printf(TEXT("[%s]"), *weaponString);
    }else{
        outMessage += FString::Printf(TEXT("[failed get weapontype]"));
    }
}