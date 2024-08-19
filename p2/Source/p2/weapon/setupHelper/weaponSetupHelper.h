// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/attachmentEnums/weaponSightEnum.h"

/**
 * This class will safe the type and the targeted attachments for a weapon
 * the entity manager will process this request, spawn a weapon if possible
 * and enable all wanted attachments if possible.
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
