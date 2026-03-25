// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "p2/player/inventory/Slots/InventorySlotBase.h"
#include "CoreMinimal.h"

class UWidgetEntityLootPayload;


class P2_API InventoryBase
{
protected:
    UCameraComponent *playerCamera = nullptr;
    bool indexActive = false;

public:
    bool IndexActive(){
        return indexActive;
    }

    void Setup(UCameraComponent *cameraIn);

    virtual bool addWeaponIfNotInInventory(Aweapon *weaponIn) = 0;

    int SizeInventory();
    bool HasItems();

    int ValidateIndex(int index);

    ///@brief current index in inventory selected
	int currentIndexNum();

	void Collect(UWidgetEntityLootPayload *payload);

    void UpdateShowWeapon(int index);
    void UpdateShowWeapon();
    void HideAll();

    bool currentIndexIsValid();
    bool indexIsValid(int i);

    Aweapon *getItemPointer();

    bool CurrentWeaponHasAimDisplacement();


    void shoot();
	void aim(bool aim);
	void releaseShoot();
    virtual void reloadWeapon() = 0;
    virtual void dropWeapon();
    virtual void dropAllWeaponsToObjectPool();
    float recoilValue();

    InventorySlotBase &CurrentSlotRefBase();
    InventorySlotBase *SlotBaseAt(int index);
    Aweapon *getItemPointerAtIndex(int index);

    virtual void selectIndex(int index);

    void AppendAllSlots(TArray<const InventorySlotBase *> &slotsOut);

protected:
    InventorySlotBase fallback;

    virtual void addToAmmunition(ammunitionEnum type, int count) = 0;
    
    

    int currentIndex = 0;

    virtual TArray<InventorySlotBase *> GetItems() = 0;
    void UpdateShowWeapon(TArray<InventorySlotBase *> &items, int indexToShow);
};