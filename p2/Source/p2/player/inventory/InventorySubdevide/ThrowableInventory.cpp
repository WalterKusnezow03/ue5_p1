#include "ThrowableInventory.h"

#include "p2/weapon/enumUtil/WeaponEnumAssetPackProxy.h"
#include "p2/weapon/throwableItem/throwableWeaponBase.h"



TArray<InventorySlotBase *> ThrowableInventory::GetItems() {
    TArray<InventorySlotBase *> items;
    for (int i = 0; i < slotVector.Num(); i++){
        items.Add(&slotVector[i]);
    }
    return items;
}


/*
ThrowableInventory::ThrowableInventory()
{
    currentIndex = 0;
    //ammunitionMap[ammunitionEnum::grenade_ammunition] = 4;
    //ammunitionMap[ammunitionEnum::molotov_ammunition] = 4;
}*/

void ThrowableInventory::selectIndex(int index){
    InventoryBase::selectIndex(index);
    DebugHelper::logMessage("ThrowableInventory::selectIndex ", index);
}

void ThrowableInventory::reloadWeapon(){
    //nothing.
}

int ThrowableInventory::currentAmmunition(){
    if(IndexActive()){
        InventorySlotThrowable &slot = CurrentSlotRefThrowable();
        return slot.AmmunitionInnerCount();
    }
    return 0;
}

InventorySlotThrowable &ThrowableInventory::CurrentSlotRefThrowable(){
    if(IndexActive()){
        return slotVector[currentIndex];
    }
    return fallbackThrowableSlot;
}

void ThrowableInventory::addToAmmunition(ammunitionEnum type, int count){
    //find or create slot.
    
    if(WeaponEnumAssetPackProxy::IsThrowable(type)){
        weaponEnum convertedFound;
        if(WeaponEnumAssetPackProxy::Convert(type, convertedFound)){
            if(InventorySlotBase *slot = FindSlotByType(convertedFound)){
                slot->reload(count);
            }else{
                //auto create aactor weapon in slot
                //auto pickup to player
                slotVector.Add(InventorySlotThrowable(convertedFound, playerCamera));

                //set player reference
                if(Aweapon *inner = slotVector.Last().GetWeaponPointer()){
                    //set player reference for shooting direction.
                    /*if(playerCamera){
                        inner->pickup(playerCamera);
                    }*/
                    

                    //hide weapon
                    inner->showWeapon(false);
                }
            }
        }
    }
}


bool ThrowableInventory::addWeaponIfNotInInventory(Aweapon *weaponIn){
    if(IsAThrowableTypeWeapon(weaponIn)){
        //try find from existing slots,
        //increase ammunition
        if(InventorySlotBase *slot = FindSlotByType(weaponIn)){
            slot->reload(1);
            return true;
        }

        //create new slot
        slotVector.Add(InventorySlotThrowable(weaponIn, playerCamera));
        selectIndex(slotVector.Num() - 1);
        return true;
    }
    return false;
}

bool ThrowableInventory::IsAThrowableTypeWeapon(Aweapon *weaponIn){
    if(weaponIn){
        if(AthrowableWeaponBase *casted = Cast<AthrowableWeaponBase>(weaponIn)){
            return true;
        }
    }
    return false;
}

InventorySlotThrowable *ThrowableInventory::FindSlotByType(Aweapon *weapon){
    if(weapon){
        return FindSlotByType(weapon->weaponType());
    }
    return nullptr;
}

InventorySlotThrowable *ThrowableInventory::FindSlotByType(weaponEnum typeWeapon){
    for (int i = 0; i < slotVector.Num(); i++){
        InventorySlotThrowable &slot = slotVector[i];
        if (slot.IsSameType(typeWeapon))
        {
            return &slot;
        }
    }
    return nullptr;
}


InventorySlotThrowable *ThrowableInventory::FindSlotByType(ammunitionEnum typeAmmunition){
    
    for (int i = 0; i < slotVector.Num(); i++){
        InventorySlotThrowable &slot = slotVector[i];
        if (slot.IsSameType(typeAmmunition))
        {
            return &slot;
        }
    }
    
    return nullptr;
}