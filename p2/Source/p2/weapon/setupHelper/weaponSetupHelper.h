// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetPlugin/gamestart/assetEnums/weaponEnum.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"

/**
 * This class will safe the type and the targeted attachments for a weapon
 * the entity manager will process this request, spawn a weapon if possible
 * and enable all wanted attachments if possible.
 * 
 */
class Aweapon;

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
	weaponAttachmentEnum getMuzzleTypeToCreate();
	weaponAttachmentEnum getGripTypeToCreate();

	//apply attachments to a weapon
	void applyAttachments(Aweapon *weaponIn);


	//set types and attachments
	void setWeaponTypeToCreate(weaponEnum typeIn);
	void setSightAttachment(weaponAttachmentEnum sightIn);
	void setMuzzleAttachment(weaponAttachmentEnum typein);
	void setGripAttachment(weaponAttachmentEnum typein);

	void SetAnyAttachmentFilterInternal(weaponAttachmentEnum typeIn);

private:
	weaponEnum typeToCreate;



	//attachments to set
	weaponAttachmentEnum sightToSet;
	weaponAttachmentEnum muzzleToSet;
	weaponAttachmentEnum gripToSet;
};
