// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "p2/weapon/weapon.h"
#include "p2/weapon/ammunitionEnum.h"
#include <map>
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

	float recoilValue();

	Aweapon *getItemPointer();

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
	std::vector<playerInventory::wslot *> weaponVector;
	int currentIndex;

	int ammunition;

	bool currentIndexIsValid();
	void showScreenMessage(FString s);

	void addToAmmunition(ammunitionEnum type, int amount);
	int getFromAmmunition(ammunitionEnum type, int amount);

	std::map<ammunitionEnum, int> ammunitionMap;
};
