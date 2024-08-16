// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/attachmentEnums/weaponSightEnum.h"

/**
 * 
 */
class P2_API weaponSetupHelper
{
public:
	weaponSetupHelper();
	~weaponSetupHelper();

	//get the targeted weapon type
	weaponEnum getWeaponTypeToCreate();

	//apply attachments to a weapon
	void applyAttachments(Aweapon *weaponIn);


	//set types and attachments
	void setWeaponTypeToCreate(weaponEnum typeIn);
	void setSightAttachment(weaponSightEnum sightIn);

private:
	weaponEnum typeToCreate;



	//attachments to set
	weaponSightEnum sightToSet;

};
