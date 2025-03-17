// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"

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
	weaponSetupHelper(const weaponSetupHelper &other);
	weaponSetupHelper& operator=(const weaponSetupHelper &other);
	~weaponSetupHelper();

	bool isSame(weaponSetupHelper &other);

	//get the targeted weapon type
	weaponEnum getWeaponTypeToCreate();
	weaponAttachmentEnum getSightTypeToCreate();

	//apply attachments to a weapon
	void applyAttachments(Aweapon *weaponIn);


	//set types and attachments
	void setWeaponTypeToCreate(weaponEnum typeIn);
	void setSightAttachment(weaponAttachmentEnum sightIn);

	static std::vector<weaponAttachmentEnum> validSights();
	static std::vector<weaponAttachmentEnum> validGrips();

	static bool isASightAttachment(weaponAttachmentEnum type);

private:
	weaponEnum typeToCreate;



	//attachments to set
	weaponAttachmentEnum sightToSet;

};
