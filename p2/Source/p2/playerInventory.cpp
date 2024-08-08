// Fill out your copyright notice in the Description page of Project Settings.

#include "weapon.h"
#include "playerInventory.h"

playerInventory::playerInventory()
{
    currentIndex = 0;
}

playerInventory::~playerInventory()
{
    while(weaponVector.size() > 0){
        wslot *w = weaponVector.at(0);
        weaponVector.erase(weaponVector.begin());
        delete w;
    }
}

playerInventory::wslot::wslot(Aweapon *in){
    this->weaponPointer = in;
}

playerInventory::wslot::~wslot(){

}
		



//inventory methods

/// @brief selects an index from the list 
/// @param index 
void playerInventory::selectIndex(int index){
    if(weaponVector.size() > 0){

        //set the index
        if(index > weaponVector.size() || index < 0){
            index = 0;
        }
        currentIndex = index;

        //enable correct weapon
        for (int i = 0; i < weaponVector.size(); i++){
            if(i == currentIndex){
                if(weaponVector.at(i)){
                    weaponVector.at(i)->show(true);
                }
            }else{
                if(weaponVector.at(i)){
                    weaponVector.at(i)->show(false);
                }
            }
        }
    }
    
}

/// @brief adds a weapon to the inventory and selects it
/// @param weaponIn new weapon to pickup
void playerInventory::addWeapon(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        weaponVector.push_back(new playerInventory::wslot(weaponIn));
        currentIndex = weaponVector.size() - 1;
        selectIndex(currentIndex);
    }
}

/// @brief reloads the current weapon if possible
void playerInventory::reloadWeapon(){
    if(currentIndexIsValid()){
        if(weaponVector.at(currentIndex) != nullptr){

            Aweapon *c = weaponVector.at(currentIndex)->weaponPointer;
            if(c){
                int mSize = c->getMagSize();
                if(ammunition - mSize > 0){
                    weaponVector.at(currentIndex)->reload(mSize);
                    ammunition -= mSize;
                }else{
                    weaponVector.at(currentIndex)->reload(ammunition);
                    ammunition = 0;
                }
            }
        }
    }
}

/// @brief adds ammunition to the inventory
/// @param ammunitionIn 
/// @param type 
void playerInventory::addAmmunition(int ammunitionIn, int type){
    ammunition += ammunitionIn;
}

/// @brief drops the current weapon if possible
void playerInventory::dropWeapon(){
    if(currentIndexIsValid()){
        playerInventory::wslot *slot = weaponVector.at(currentIndex);
        slot->drop();
        weaponVector.erase(weaponVector.begin() + currentIndex);
        delete slot;
        currentIndex -= 1;
        selectIndex(currentIndex);
    }
}

/// @brief shoots the current weapon if possible
void playerInventory::shoot(){
    if(currentIndexIsValid()){
        if(weaponVector.at(currentIndex)){
            weaponVector.at(currentIndex)->shoot();
        }
    }
}

/// @brief aim the current weapon
/// @param aim 
void playerInventory::aim(bool aim){
    if(currentIndexIsValid()){
        weaponVector.at(currentIndex)->aim(aim);
    }
}

/// @brief release shoot for the current weapon
void playerInventory::releaseShoot(){
    if(currentIndexIsValid()){
        weaponVector.at(currentIndex)->releaseShoot();
    }
}

/// @brief will return if the current weapon index is valid,
/// and if any items are in the list 
/// @return 
bool playerInventory::currentIndexIsValid(){
    if(currentIndex < weaponVector.size() && (weaponVector.size() > 0) && currentIndex >= 0){
        if(weaponVector.at(currentIndex) != nullptr){
            return true;
        }
    }
    return false;
}







//slot methods

/// @brief shoots the weapon
void playerInventory::wslot::shoot(){
    if(weaponPointer != nullptr){
        weaponPointer->shoot();
    }
}

/// @brief reload the weapon
/// @param amount 
void playerInventory::wslot::reload(int amount){
    weaponPointer->reload(amount);
}

/// @brief mag size of the weapon
/// @return 
int playerInventory::wslot::getMagSize(){
    if(weaponPointer){
        return weaponPointer->getMagSize();
    }
    return 0;
}

/// @brief shows the weapon or not based on passed bool
/// @param show 
void playerInventory::wslot::show(bool show){
    if(weaponPointer != nullptr){
        weaponPointer->showWeapon(show);
    }
}

/// @brief drops the weapon, "this" will be deleted from inventory 
void playerInventory::wslot::drop(){
    if(weaponPointer != nullptr){
        weaponPointer->dropweapon();
    }
}

/// @brief aim or not aim the weapon
/// @param aim 
void playerInventory::wslot::aim(bool aim){
    if(weaponPointer){
        weaponPointer->aim(aim);
    }
}

/// @brief releases left mouse on weapon (holding trigger)
void playerInventory::wslot::releaseShoot(){
    if(weaponPointer){
        weaponPointer->releaseShoot();
    }
}