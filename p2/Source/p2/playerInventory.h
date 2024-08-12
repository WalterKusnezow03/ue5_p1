// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "weapon.h"
#include "CoreMinimal.h"

/**
 * 
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
	void selectIndex(int index);

	void addWeapon(Aweapon *weaponIn);
	void reloadWeapon();
	void addAmmunition(int ammunition, int type);

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
	};
	std::vector<playerInventory::wslot *> weaponVector;
	int currentIndex;

	int ammunition;

	bool currentIndexIsValid();
	void showScreenMessage(FString s);
};
