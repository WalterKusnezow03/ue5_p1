// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"
#include <map>
#include "p2/player/inventory/InventorySlot.h"
#include "CoreMinimal.h"

class UWidgetEntityLootPayload;

/**
 * must be refactured to carried item!
 */
class P2_API PlayerInventory
{
public:
	PlayerInventory();
	~PlayerInventory();

	void shoot();
	void aim(bool aim);
	void releaseShoot();
	void dropWeapon();

	bool CurrentWeaponHasAimDisplacement();

	/// @brief swicthes to an index, hides the current weapon
	/// @param index 
	void selectIndex(int index);

	void dropAllWeaponsToObjectPool();

	void addWeaponIfNotInInventory(Aweapon *weaponIn);
	void reloadWeapon();
	

	float recoilValue();

	Aweapon *getItemPointer();
	Aweapon *getItemPointerAtIndex(int index);

	//ui interface
	int currentAmmunition();
	int currentLeftAmmnutionInMag();

	

	///@brief current index in inventory selected
	int currentIndexNum();

	void Collect(UWidgetEntityLootPayload *payload);

private:
	void UpdateShowWeapon();
	InventorySlot &CurrentSlotRef();
	int ValidateIndex(int index);

	/// @brief stored on heap because copy constructor must be const, NOT wanted here!
	TArray<InventorySlot> weaponVector;
	InventorySlot fallback;
	int currentIndex;

	int ammunition;

	bool currentIndexIsValid();
	bool indexIsValid(int index);

	void addToAmmunition(ammunitionEnum type, int amount);
	int getFromAmmunition(ammunitionEnum type, int amount);

	std::map<ammunitionEnum, int> ammunitionMap;

	

	bool alreadyInInventory(Aweapon *weaponIn, int &foundindex);
};
