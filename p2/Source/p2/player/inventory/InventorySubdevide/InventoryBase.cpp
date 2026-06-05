#include "InventoryBase.h"
#include "p2/entities/widgetPayloads/WidgetEntityLootPayload.h"

void InventoryBase::Setup(UCameraComponent *cameraIn){
    playerCamera = cameraIn;
}

bool InventoryBase::HasItems(){
    return SizeInventory() > 0;
}

int InventoryBase::ValidateIndex(int index){
    index = std::max(index, 0);
    index = std::min(index, SizeInventory() - 1);
    return index;
}



int InventoryBase::currentIndexNum(){
    return currentIndex;
}


void InventoryBase::selectIndex(int index){
    if(SizeInventory() > 0){
        currentIndex = ValidateIndex(index);
        UpdateShowWeapon();
        indexActive = true;
    }
}


void InventoryBase::UpdateShowWeapon(){
    TArray<InventorySlotBase *> items = GetItems();
    UpdateShowWeapon(items, currentIndex);
}

void InventoryBase::UpdateShowWeapon(int index){
    TArray<InventorySlotBase *> items = GetItems();
    UpdateShowWeapon(items, index);
}

void InventoryBase::UpdateShowWeapon(TArray<InventorySlotBase *> &items, int indexToShow){
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("InventoryBase::UpdateShowWeapon %d of %d"),
            indexToShow, items.Num()
        )
    );
    for (int i = 0; i < items.Num(); i++){
        bool showFlag = i == indexToShow;
        if(InventorySlotBase *current = items[i]){
            current->show(showFlag);
        }
    }
}

void InventoryBase::HideAll(){
    TArray<InventorySlotBase *> items = GetItems();
    UpdateShowWeapon(items, -1);
    indexActive = false;
}

bool InventoryBase::currentIndexIsValid(){
    return indexIsValid(currentIndex);
}

bool InventoryBase::indexIsValid(int index){
    return index >= 0 && index < SizeInventory();
}


void InventoryBase::Collect(UWidgetEntityLootPayload *payload){
    if(payload){
        ammunitionEnum type = payload->GetTypeAmmunition();
        int count = payload->GetAmmunitionAmount();
        addToAmmunition(type, count);
    }
}




/// @brief shoots the current weapon if possible
void InventoryBase::shoot(){
    if(!indexActive){
        return;
    }

    if(currentIndexIsValid()){
        CurrentSlotRefBase().shoot();
    }
}

/// @brief apply recoil value to player just before shooting, will return value based on canshoot
/// @return returns a value != 0 if CAN SHOOT, apply immidiatly
float InventoryBase::recoilValue(){
    if(!indexActive){
        return 0.0f;
    }
    if(currentIndexIsValid()){
        return CurrentSlotRefBase().recoilValue();
    }
    return 0.0f;
}


/// @brief aim the current weapon
/// @param aim 
void InventoryBase::aim(bool aim){
    if(!indexActive){
        return;
    }
    if(currentIndexIsValid()){
        CurrentSlotRefBase().aim(aim);
    }
}

/// @brief release shoot for the current weapon
void InventoryBase::releaseShoot(){
    if(!indexActive){
        return;
    }
    if(currentIndexIsValid()){
        CurrentSlotRefBase().releaseShoot();
    }
}

int InventoryBase::SizeInventory(){
    return GetItems().Num();
}



/// @brief current weapon by index, if an issue occured, nullptr is
/// returned. You are NOT allowed to delete this!
/// @return pointer or nullptr if none found
Aweapon *InventoryBase::getItemPointer(){
    if(!indexActive){
        return nullptr;
    }
    //return weaponPointer;
    if(currentIndexIsValid()){
        return getItemPointerAtIndex(currentIndex);
    }
    return nullptr;
}

Aweapon *InventoryBase::getItemPointerAtIndex(int index){
    if(indexIsValid(index)){
        if(InventorySlotBase *slot = SlotBaseAt(index)){
            return slot->GetWeaponPointer();
        }
    }
    return nullptr;
}


bool InventoryBase::CurrentWeaponHasAimDisplacement(){
    if(!indexActive){
        return false;
    }
    if(Aweapon *item = getItemPointer()){
        return item->HasAimingDisplacement();
    }
    return false;
}




/// @brief drops the current weapon if possible
void InventoryBase::dropWeapon(){
    if(!indexActive){
        return;
    }
    if(currentIndexIsValid()){
        CurrentSlotRefBase().drop();
    }
}


void InventoryBase::dropAllWeaponsToObjectPool(){
    TArray<InventorySlotBase *> allItems = GetItems();
    for (int i = 0; i < allItems.Num(); i++){
        if(InventorySlotBase *current = allItems[i]){
            Aweapon *weaponPtr = current->GetWeaponPointer();
            if(weaponPtr){
                weaponPtr->dropToObjectPool();
            }
            current->ResetWeaponPointer(); // weaponPointer = nullptr;
        }
    }
}





InventorySlotBase &InventoryBase::CurrentSlotRefBase(){
    if(InventorySlotBase *found = SlotBaseAt(currentIndex)){
        return *found;
    }
    return fallback;
}

InventorySlotBase *InventoryBase::SlotBaseAt(int index){
    TArray<InventorySlotBase *> items = GetItems();
    if(index >= 0 && index < items.Num()){
        if(InventorySlotBase * item = items[index]){
            return item;
        }
    }
    return nullptr;
}


void InventoryBase::AppendAllSlots(TArray<const InventorySlotBase *> &slotsOut){
    TArray<InventorySlotBase *> items = GetItems();
    for (int i = 0; i < items.Num(); i++){
        if(InventorySlotBase *ptr = items[i]){
            slotsOut.Add(ptr);
        }
    }
}
