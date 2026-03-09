// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerInventory.h"
#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"
#include "p2/entities/widgetPayloads/WidgetEntityLootPayload.h"

PlayerInventory::PlayerInventory()
{
    currentIndex = 0;
    ammunition = 1000; //testing

    ammunitionMap[ammunitionEnum::assaultrifle556] = 100;
    ammunitionMap[ammunitionEnum::pistol9] = 50;
    ammunitionMap[ammunitionEnum::heavy762] = 200;

    ammunitionMap[ammunitionEnum::grenade_ammunition] = 5;
    ammunitionMap[ammunitionEnum::rpg_ammunition] = 5;
    ammunitionMap[ammunitionEnum::molotov_ammunition] = 5;
}

PlayerInventory::~PlayerInventory()
{
    
}


//inventory methods

/// @brief selects an index from the list 
/// @param index 
void PlayerInventory::selectIndex(int index){
    if(weaponVector.Num() > 0){
        currentIndex = ValidateIndex(index);
        UpdateShowWeapon();
    }
}

int PlayerInventory::ValidateIndex(int index){
    if(index >= weaponVector.Num() || index < 0){
        index = 0;
    }
    return index;
}

void PlayerInventory::UpdateShowWeapon(){
    //enable correct weapon
    for (int i = 0; i < weaponVector.Num(); i++){
        InventorySlot &current = weaponVector[i];
        bool showTrue = i == currentIndex;
        weaponVector[i].show(showTrue);
    }
}







/// @brief adds a weapon to the inventory and selects it, if not contained yet
/// @param weaponIn new weapon to pickup
void PlayerInventory::addWeaponIfNotInInventory(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        int foundIndexToSelect = 0;
        if (!alreadyInInventory(weaponIn, foundIndexToSelect))
        {
            weaponVector.Add(InventorySlot(weaponIn));
            currentIndex = weaponVector.Num() - 1;
            selectIndex(currentIndex);
        }else{
            selectIndex(foundIndexToSelect);
        }
    }
}

///@brief checks if a weapon is already in inventory
bool PlayerInventory::alreadyInInventory(Aweapon *weaponIn, int &foundIndex){
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
void PlayerInventory::reloadWeapon(){
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
void PlayerInventory::addToAmmunition(ammunitionEnum type, int amount){
    if(amount > 0){
        ammunitionMap[type] += amount;
    }
}

/// @brief returns the targeted ammunition for a type if anough left or the left over
/// @param type ammunition type
/// @param amount amount to get
/// @return between inclusive 0 and amount passed
int PlayerInventory::getFromAmmunition(ammunitionEnum type, int amount){
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



/// @brief will return if the current weapon index is valid and not nullptr,
/// and if any items are in the list 
/// @return 
bool PlayerInventory::currentIndexIsValid(){
    return indexIsValid(currentIndex);
}

bool PlayerInventory::indexIsValid(int index){
    return index >= 0 && index < weaponVector.Num();
}

InventorySlot &PlayerInventory::CurrentSlotRef(){
    if(currentIndexIsValid()){
        return weaponVector[currentIndex];
    }
    return fallback;
}

/**
 * player api
 */


/// @brief drops the current weapon if possible
void PlayerInventory::dropWeapon(){
    if(currentIndexIsValid()){
        InventorySlot &current = CurrentSlotRef();
        current.drop();
        weaponVector.RemoveAt(currentIndex);
        currentIndex -= 1;
        selectIndex(currentIndex);
    }
}


void PlayerInventory::dropAllWeaponsToObjectPool(){
    if(weaponVector.Num() <= 0){
        return;
    }

    for(int i = 0; i < weaponVector.Num(); i++){
        InventorySlot &current = weaponVector[i];
        if(Aweapon *weapon = current.weaponPointer){
            weapon->dropToObjectPool();
        }
        current.weaponPointer = nullptr;
    }
    weaponVector.Empty();
}

/// @brief shoots the current weapon if possible
void PlayerInventory::shoot(){
    if(currentIndexIsValid()){
        InventorySlot &current = CurrentSlotRef();
        current.shoot();
        /*if(weaponVector.at(currentIndex)){
            weaponVector.at(currentIndex)->shoot();

            FString Message = FString::Printf(TEXT("weapon: %d"), currentIndex);
            //showScreenMessage(Message);
        }*/
    }
}

/// @brief apply recoil value to player just before shooting, will return value based on canshoot
/// @return returns a value != 0 if CAN SHOOT, apply immidiatly
float PlayerInventory::recoilValue(){
    if(currentIndexIsValid()){
        return CurrentSlotRef().recoilValue();
    }
    return 0.0f;
}


/// @brief aim the current weapon
/// @param aim 
void PlayerInventory::aim(bool aim){
    if(currentIndexIsValid()){
        CurrentSlotRef().aim(aim);
    }
}

/// @brief release shoot for the current weapon
void PlayerInventory::releaseShoot(){
    if(currentIndexIsValid()){
        CurrentSlotRef().releaseShoot();
    }
}





/// @brief current weapon by index, if an issue occured, nullptr is
/// returned. You are NOT allowed to delete this!
/// @return pointer or nullptr if none found
Aweapon *PlayerInventory::getItemPointer(){
    //return weaponPointer;
    if(currentIndexIsValid()){
        return getItemPointerAtIndex(currentIndex);
    }
    return nullptr;
}

///@brief returns the weapon pointer at an index, if its valid, nullptr if not
Aweapon *PlayerInventory::getItemPointerAtIndex(int index){
    if(indexIsValid(index)){
        return weaponVector[index].weaponPointer;
    }
    return nullptr;
}


int PlayerInventory::currentIndexNum(){
    return currentIndex;
}

bool PlayerInventory::CurrentWeaponHasAimDisplacement(){
    if(Aweapon *item = getItemPointer()){
        return item->HasAimingDisplacement();
    }
    return false;
}


// --- APi for ui changes ---
int PlayerInventory::currentAmmunition(){
    if(Aweapon *currentWeapon = getItemPointer()){
        return currentWeapon->getBulletsInMag();
    }
    return 0;
}
int PlayerInventory::currentLeftAmmnutionInMag(){
    if(Aweapon *currentWeapon = getItemPointer()){
        return currentWeapon->getMagSize();
    }
    return 0;
}





void PlayerInventory::Collect(UWidgetEntityLootPayload *payload){
    if(payload){
        ammunitionEnum type = payload->GetTypeAmmunition();
        int count = payload->GetAmmunitionAmount();
        addToAmmunition(type, count);
    }
}

