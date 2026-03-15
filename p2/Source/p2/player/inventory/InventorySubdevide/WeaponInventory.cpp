#include "WeaponInventory.h"

// Fill out your copyright notice in the Description page of Project Settings.
#include "p2/weapon/weapon.h"
#include "p2/weapon/throwableItem/throwableWeaponBase.h"
#include "p2/weapon/ammunitionEnum.h"
#include "p2/entities/widgetPayloads/WidgetEntityLootPayload.h"

/*
WeaponInventory::WeaponInventory()
{
    currentIndex = 0;
    ammunition = 1000; //testing

    ammunitionMap[ammunitionEnum::assaultrifle556] = 100;
    ammunitionMap[ammunitionEnum::pistol9] = 50;
    ammunitionMap[ammunitionEnum::heavy762] = 200;
}*/

TArray<InventorySlotBase *> WeaponInventory::GetItems(){
    TArray<InventorySlotBase *> items;
    for (int i = 0; i < weaponVector.Num(); i++){
        items.Add(&weaponVector[i]);
    }
    return items;
}





/// @brief adds a weapon to the inventory and selects it, if not contained yet
/// @param weaponIn new weapon to pickup
bool WeaponInventory::addWeaponIfNotInInventory(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        if(AthrowableWeaponBase *casted = Cast<AthrowableWeaponBase>(weaponIn)){
            return false;
        }

        int foundIndexToSelect = 0;
        if (!alreadyInInventory(weaponIn, foundIndexToSelect))
        {
            InventorySlot newSlot;
            newSlot.weaponPointer = weaponIn;
            weaponVector.Add(newSlot);
            currentIndex = weaponVector.Num() - 1;
            selectIndex(currentIndex);
            return true;
        }else{
            selectIndex(foundIndexToSelect);
            return true;
        }
    }
    return false;
}

///@brief checks if a weapon is already in inventory
bool WeaponInventory::alreadyInInventory(Aweapon *weaponIn, int &foundIndex){
    if(weaponIn != nullptr){
        for (int i = 0; i < weaponVector.Num(); i++)
        {
            InventorySlot &current = weaponVector[i];
            if(current.IsSamePointer(weaponIn)){
                foundIndex = i;
                return true;
            }
        }
    }
    return false;
}

/// @brief reloads the current weapon if possible
void WeaponInventory::reloadWeapon(){
    if(currentIndexIsValid()){
        InventorySlot &slot = weaponVector[currentIndex];
        Aweapon *weapon = slot.weaponPointer;
        if(weapon){

            //reload if reload possible
            if(weapon->canReload()){

                //new ---> testing needed with debug messages
                ammunitionEnum type = weapon->getAmmunitionType();
                int leftInMag = weapon->getBulletsInMag();
                int magSize = weapon->getMagSize() - leftInMag;
                
                
                addToAmmunition(type, leftInMag);

                //werden in waffe nicht entfernt, aufstocken auf voll
                slot.reload(getFromAmmunition(type, magSize));


            }
        }
    }
}

/// @brief adds amuntion to the correct map index
/// @param type ammunition type from the enum
/// @param amount amount to add to the map
void WeaponInventory::addToAmmunition(ammunitionEnum type, int amount){
    if(amount > 0){
        ammunitionMap[type] += amount;
    }
}

/// @brief returns the targeted ammunition for a type if anough left or the left over
/// @param type ammunition type
/// @param amount amount to get
/// @return between inclusive 0 and amount passed
int WeaponInventory::getFromAmmunition(ammunitionEnum type, int amount){
    if(amount > 0){
        int available = ammunitionMap[type];
        if(available > 0){
            if(available >= amount){
                available -= amount; //update count 
                ammunitionMap[type] = available;
                return amount;
            }else{
                ammunitionMap[type] = 0; //set to 0
                return available; //return whats left
            }
        }
    }
    return 0;
}






/**
 * player api
 */

void WeaponInventory::dropWeapon(){
    InventoryBase::dropWeapon();
    if(currentIndexIsValid()){
        weaponVector.RemoveAt(currentIndex);
        currentIndex -= 1;
        selectIndex(currentIndex);
    }
}

void WeaponInventory::dropAllWeaponsToObjectPool(){
    InventoryBase::dropAllWeaponsToObjectPool();
    weaponVector.Empty();
}






// --- APi for ui changes ---
int WeaponInventory::currentAmmunition(){
    if(Aweapon *currentWeapon = getItemPointer()){
        return currentWeapon->getBulletsInMag();
    }
    return 0;
}
int WeaponInventory::currentLeftAmmnutionInMag(){
    if(Aweapon *currentWeapon = getItemPointer()){
        return currentWeapon->getMagSize();
    }
    return 0;
}





