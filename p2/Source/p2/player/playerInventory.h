// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"
#include <map>
#include "CoreMinimal.h"

/**
 * must be refactured to carried item!
 */
class P2_API playerInventory
{
public:
	playerInventory();
	~playerInventory();

	void shoot();
	void aim(bool aim);
	void releaseShoot();
	void dropWeapon();

	/// @brief swicthes to an index, hides the current weapon
	/// @param index 
	void selectIndex(int index);

	void dropAllWeaponsToObjectPool();

	void addWeaponIfNotInInventory(Aweapon *weaponIn);
	void reloadWeapon();
	void addAmmunition(int ammunition, int type);

	float recoilValue();

	Aweapon *getItemPointer();
	Aweapon *getItemPointerAtIndex(int index);

	//ui interface
	int currentAmmunition();
	int currentLeftAmmnutionInMag();

	

	///@brief current index in inventory selected
	int currentIndexNum();

private:
	class wslot{
		public:
			wslot(Aweapon *in);
			~wslot();
			Aweapon *weaponPointer;
			
			void shoot();
			void reload(int amount);
			void drop();
			void show(bool show);
			void aim(bool aim);
			void releaseShoot();
			int getMagSize();
			int getBulletsInMag();
			float recoilValue();
	};


	/// @brief stored on heap because copy constructor must be const, NOT wanted here!
	std::vector<playerInventory::wslot *> weaponVector;
	int currentIndex;

	int ammunition;

	bool currentIndexIsValid();
	bool indexIsValid(int index);

	void addToAmmunition(ammunitionEnum type, int amount);
	int getFromAmmunition(ammunitionEnum type, int amount);

	std::map<ammunitionEnum, int> ammunitionMap;

	

	bool alreadyInInventory(Aweapon *weaponIn, int &foundindex);
};
