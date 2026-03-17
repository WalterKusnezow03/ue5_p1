// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"
#include <map>
#include "p2/player/inventory/InventorySubdevide/InventoryBase.h"
#include "p2/player/inventory/Slots/InventorySlot.h"
#include "CoreMinimal.h"

class UWidgetEntityLootPayload;


class P2_API WeaponInventory : public InventoryBase
{
public:

	
	

	virtual void dropAllWeaponsToObjectPool() override;

	virtual bool addWeaponIfNotInInventory(Aweapon *weaponIn) override;
	void reloadWeapon();
	

	//ui interface
	int currentAmmunition();
	int currentLeftAmmnutionInMag();

    virtual void dropWeapon() override;

    virtual void selectIndex(int index) override;
    TArray<InventorySlotBase *> GetItems() override;
    void addToAmmunition(ammunitionEnum type, int amount);

    FString ToString();

private:
    /// @brief stored on heap because copy constructor must be const, NOT wanted here!
	TArray<InventorySlot> weaponVector;
	InventorySlot fallback;
	int currentIndex;

	int ammunition;

    void reloadWeaponSlot(InventorySlotBase &slot);
    void reloadWeapon(Aweapon *weapon);

    int getFromAmmunition(ammunitionEnum type, int amount);

	std::map<ammunitionEnum, int> ammunitionMap;

	

	bool alreadyInInventory(Aweapon *weaponIn, int &foundindex);
};
