// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerInventory.h"
#include "p2/weapon/weapon.h"
#include "p2/weapon/throwableItem/throwableWeaponBase.h"
#include "p2/weapon/ammunitionEnum.h"
#include "p2/entities/widgetPayloads/WidgetEntityLootPayload.h"
#include "DebugPlugin/DebugHelper.h"

PlayerInventory::PlayerInventory()
{
    currentIndex = 0;
    
    weaponInventory.addToAmmunition(ammunitionEnum::assaultrifle556, 200);
    weaponInventory.addToAmmunition(ammunitionEnum::pistol9, 50);
    weaponInventory.addToAmmunition(ammunitionEnum::rpg_ammunition, 4);

}

PlayerInventory::~PlayerInventory()
{
    
}

void PlayerInventory::logMessage(){
    FString message = FString::Printf(
        TEXT("PlayerInventory weapons(%d) throwables(%d)"),
        weaponInventory.SizeInventory(),
        throwableInventory.SizeInventory()
    );
    DebugHelper::logMessage(message);
}

void PlayerInventory::Setup(UCameraComponent *cameraIn){
    weaponInventory.Setup(cameraIn);
    throwableInventory.Setup(cameraIn);
}

int PlayerInventory::TotalSizeInventory(){
    return 
    weaponInventory.SizeInventory() + 
    throwableInventory.SizeInventory();
}


//inventory methods

/// @brief selects an index from the list 
/// @param index 
void PlayerInventory::selectIndex(int index){
    //DebugHelper::logMessage("PlayerInventory::selectIndex ", index);
    //DebugHelper::showScreenMessage("PlayerInventory::selectIndex ", index, FColor::Red);
    if (weaponInventory.HasItems() || throwableInventory.HasItems())
    {
        int validated = ValidateIndex(index);
        if(validated != currentIndex){
            currentIndex = validated;
            //DebugHelper::logMessage("PlayerInventory::validate selectIndex ", currentIndex);
            UpdateShowWeapon();
        }
    }
    logMessage();
}

int PlayerInventory::ValidateIndex(int index){
    if(index >= TotalSizeInventory() || index < 0){
        index = 0;
    }
    return index;
}

void PlayerInventory::UpdateShowWeapon(){
    weaponInventory.HideAll();
    throwableInventory.HideAll();

    if(currentIndex >= 0 && currentIndex < weaponInventory.SizeInventory()){
        DebugHelper::logMessage("PlayerInventory::weaponInventory select index ", currentIndex);
        weaponInventory.selectIndex(currentIndex);
        return;
    }
    int indexThrowableInventory = IndexInThrowableInventory(currentIndex);
    if(indexThrowableInventory >= 0 && indexThrowableInventory < throwableInventory.SizeInventory()){
        DebugHelper::logMessage("PlayerInventory::throwableInventory select index ", indexThrowableInventory);
        throwableInventory.selectIndex(currentIndex);
        return;
    }
}

int PlayerInventory::IndexInWeaponInventory(int index){
    return index;
}
int PlayerInventory::IndexInThrowableInventory(int index){
    return index - weaponInventory.SizeInventory();
}





/// @brief adds a weapon to the inventory and selects it, if not contained yet
/// @param weaponIn new weapon to pickup
void PlayerInventory::addWeaponIfNotInInventory(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        if(weaponInventory.addWeaponIfNotInInventory(weaponIn)){
            currentIndex = weaponInventory.currentIndexNum();
            return;
        }
        if(throwableInventory.addWeaponIfNotInInventory(weaponIn)){
            currentIndex = 
            weaponInventory.SizeInventory() +
            throwableInventory.currentIndexNum();
            return;
        }
    }
}


/// @brief reloads the current weapon if possible
void PlayerInventory::reloadWeapon(){
    weaponInventory.reloadWeapon();
    throwableInventory.reloadWeapon();
}





/// @brief will return if the current weapon index is valid and not nullptr,
/// and if any items are in the list 
/// @return 
bool PlayerInventory::currentIndexIsValid(){
    return indexIsValid(currentIndex);
}

bool PlayerInventory::indexIsValid(int index){
    return index >= 0 && index < TotalSizeInventory();
}


/**
 * player api
 */


/// @brief drops the current weapon if possible
void PlayerInventory::dropWeapon(){
    if(currentIndexIsValid()){
        int previousSize = TotalSizeInventory();
        weaponInventory.dropWeapon();
        throwableInventory.dropWeapon();
        if(previousSize > TotalSizeInventory()){
            currentIndex -= 1;
            selectIndex(currentIndex);
        }
    }
}


void PlayerInventory::dropAllWeaponsToObjectPool(){
    weaponInventory.dropAllWeaponsToObjectPool();
    throwableInventory.dropAllWeaponsToObjectPool();
}

/// @brief shoots the current weapon if possible
void PlayerInventory::shoot(){
    weaponInventory.shoot();
    throwableInventory.shoot();
}

/// @brief apply recoil value to player just before shooting, will return value based on canshoot
/// @return returns a value != 0 if CAN SHOOT, apply immidiatly
float PlayerInventory::recoilValue(){
    if(weaponInventory.IndexActive()){
        return weaponInventory.recoilValue();
    }
    if(throwableInventory.IndexActive()){
        return throwableInventory.recoilValue();
    }
    return 0.0f;
}

/// @brief aim the current weapon
/// @param aim 
void PlayerInventory::aim(bool aim){
    weaponInventory.aim(aim);
    throwableInventory.aim(aim);
}

/// @brief release shoot for the current weapon
void PlayerInventory::releaseShoot(){
    weaponInventory.releaseShoot();
    throwableInventory.releaseShoot();
}





/// @brief current weapon by index, if an issue occured, nullptr is
/// returned. You are NOT allowed to delete this!
/// @return pointer or nullptr if none found
Aweapon *PlayerInventory::getItemPointer(){
    if(weaponInventory.IndexActive()){
        return weaponInventory.getItemPointer();
    }
    if(throwableInventory.IndexActive()){
        return throwableInventory.getItemPointer();
    }
    return nullptr;
}


Aweapon *PlayerInventory::getItemPointerAtIndex(int index){
    if(index >= 0 && index < TotalSizeInventory()){
        int i = IndexInWeaponInventory(index);
        if(i >= 0 && i < weaponInventory.SizeInventory()){
            return weaponInventory.getItemPointerAtIndex(i);
        }

        i = IndexInThrowableInventory(index);
        if(i >= 0 && i < throwableInventory.SizeInventory()){
            return throwableInventory.getItemPointerAtIndex(i);
        }
    }
    return nullptr;
}

bool PlayerInventory::CurrentItemIsThrowable(){
    if(throwableInventory.IndexActive()){
        return true;
    }
    return false;
}

Aweapon *PlayerInventory::GetCurrenThrowablePointer(){
    if(CurrentItemIsThrowable()){
        InventorySlotBase &slot = throwableInventory.CurrentSlotRefBase();
        return slot.weaponPointer;
    }
    return nullptr;
}


bool PlayerInventory::CurrentWeaponHasAimDisplacement(){
    if(weaponInventory.IndexActive()){
        return weaponInventory.CurrentWeaponHasAimDisplacement();
    }
    if(throwableInventory.IndexActive()){
        return throwableInventory.CurrentWeaponHasAimDisplacement();
    }
    return false;
}


// --- APi for ui changes ---
int PlayerInventory::currentAmmunition(){
    if(throwableInventory.IndexActive()){
        return throwableInventory.currentAmmunition();
    }
    if(weaponInventory.IndexActive()){
        return weaponInventory.currentAmmunition();
    }
    return 0;
}
int PlayerInventory::currentLeftAmmnutionInMag(){
    if(throwableInventory.IndexActive()){
        return currentAmmunition();
    }
    if(weaponInventory.IndexActive()){
        return weaponInventory.currentLeftAmmnutionInMag();
    }
    return 0;
}





void PlayerInventory::Collect(UWidgetEntityLootPayload *payload){
    if(payload){
        throwableInventory.Collect(payload);
        weaponInventory.Collect(payload);
    }
}

