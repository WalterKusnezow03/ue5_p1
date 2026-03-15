#include "InventorySlotThrowable.h"
#include "p2/weapon/enumUtil/WeaponEnumAmmunitionConverter.h"


#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"

#include "DebugPlugin/DebugHelper.h"

InventorySlotThrowable::InventorySlotThrowable(Aweapon *weaponIn, UCameraComponent *cameraIn){
    if(weaponIn){
        if(AthrowableWeaponBase *casted = Cast<AthrowableWeaponBase>(weaponIn)){
            weaponPointer = weaponIn;
            typeThrowable = weaponPointer->weaponType();
        }
    }
    camera = cameraIn;
}

InventorySlotThrowable::InventorySlotThrowable(weaponEnum typeEnum, UCameraComponent *cameraIn){
    typeThrowable = typeEnum;
    camera = cameraIn;
    replaceInstance();
}

bool InventorySlotThrowable::IsSameType(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        return IsSameType(weaponIn->weaponType());
    }
    return false;
}

bool InventorySlotThrowable::IsSameType(weaponEnum typeWeaponEnum){
    if(weaponPointer != nullptr){
        return typeWeaponEnum == weaponPointer->weaponType();
    }
    return typeWeaponEnum == typeThrowable;
}

bool InventorySlotThrowable::IsSameType(ammunitionEnum typeAmmunition){
    WeaponEnumAmmunitionConverter converter;
    weaponEnum typeFound;
    if (converter.Convert(typeAmmunition, typeFound)){
        return IsSameType(typeFound);
    }
    return false;
}

/// @brief reload the weapon
/// @param amount 
void InventorySlotThrowable::reload(int amount){
    innerAmmunition += amount;
}

/// @brief mag size of the weapon
/// @return 
int InventorySlotThrowable::getMagSize(){
    return 1;
}

int InventorySlotThrowable::getBulletsInMag(){
    if(weaponPointer != nullptr){
        return 1;
    }
    return 0;
}


/// @brief releases left mouse on weapon (holding trigger)
void InventorySlotThrowable::releaseShoot(){
    InventorySlotBase::releaseShoot();
    drop();
    DebugHelper::logMessage("InventorySlotThrowable::releaseShoot()");
    TryReloadOnReleaseShoot();
}

void InventorySlotThrowable::TryReloadOnReleaseShoot(){
    if(innerAmmunition > 0){
        innerAmmunition--;
        replaceInstance();
    }
}

void InventorySlotThrowable::replaceInstance(){//new instance if enough ammunition left
    if(!camera){
        DebugHelper::logMessage("InventorySlotThrowable::replace instance failed, no camera!");
    }
    
    if(EntityManager *manager = AworldLevel::entityManager()){
        if(UWorld *world = AworldLevel::GetWorldPointer()){
            weaponPointer = manager->spawnAweapon(
                world,
                typeThrowable
            );
            if(weaponPointer){
                weaponPointer->pickup(camera);
                DebugHelper::logMessage("InventorySlotThrowable::replaced instance");
            }
            
        }
    }
}




/// @brief apply recoil value to player just before shooting, will return value based on canshoot
/// @return returns a value != 0 if CAN SHOOT, apply immidiatly
float InventorySlotThrowable::recoilValue(){
    return 0.0f;
}


