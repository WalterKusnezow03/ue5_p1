// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"
#include <map>
#include "p2/player/inventory/Slots/InventorySlot.h"
#include "Camera/CameraComponent.h"

#include "p2/player/inventory/InventorySubdevide/WeaponInventory.h"
#include "p2/player/inventory/InventorySubdevide/ThrowableInventory.h"

#include "CoreMinimal.h"

class UWidgetEntityLootPayload;

/**
 * must be refactured to carried item!
 */
class P2_API PlayerInventory
{

protected:
	WeaponInventory weaponInventory;
	ThrowableInventory throwableInventory;

public:
	PlayerInventory();
	~PlayerInventory();

	void Setup(UCameraComponent *cameraIn);

	void shoot();
	void aim(bool aim);
	void releaseShoot();
	void dropWeapon();

	
	Aweapon *GetCurrenThrowablePointer();

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
	bool CurrentItemIsThrowable();

	//ui interface
	int currentAmmunition();
	int currentLeftAmmnutionInMag();

	

	///@brief current index in inventory selected
	int currentIndexNum(){
		return currentIndex;
	}

	void Collect(UWidgetEntityLootPayload *payload);

private:
	void logMessage();

	int TotalSizeInventory();
	void UpdateShowWeapon();
	InventorySlot &CurrentSlotRef();
	int ValidateIndex(int index);

	
	int currentIndex;

	bool currentIndexIsValid();
	bool indexIsValid(int index);

	int IndexInWeaponInventory(int index);
	int IndexInThrowableInventory(int index);

	bool alreadyInInventory(Aweapon *weaponIn, int &foundindex);
};
