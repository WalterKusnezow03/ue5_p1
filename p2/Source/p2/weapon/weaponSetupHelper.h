// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weaponEnum.h"

/**
 * 
 */
class P2_API weaponSetupHelper
{
public:
	weaponSetupHelper();
	~weaponSetupHelper();



	//apply attachments to a weapon
	void applyAttachments(Aweapon *weaponIn);

private:
	weaponEnum typeToCreate;

	weaponEnum getWeaponType();


};
