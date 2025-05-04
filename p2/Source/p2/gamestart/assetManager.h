// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/gamestart/TAssetManager/assetManagerGeneric.h"
#include "p2/gamestart/assetEnums/entityEnum.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/particleSystem/particleEnum.h"
#include "p2/throwableItems/throwableEnum.h"
#include "p2/gamestart/assetEnums/textureEnum.h"
#include <map>

/**
 * 
 */
class P2_API assetManager
{
public:
	static void EndGame();
	static assetManager *instance();
	~assetManager();


	//entity
	UClass *findBp(entityEnum type);
	void addBp(entityEnum type, UClass *asset);


	//weapon
	UClass *findBp(weaponEnum type);
	void addBp(weaponEnum type, UClass *asset);

	//throwables
	UClass *findBp(throwableEnum type);
	void addBp(throwableEnum type, UClass *asset);

	//particles
	UClass *findBp(particleEnum type);
	void addBp(particleEnum type, UClass *asset);


	//all materials
	UMaterialInterface *findMaterial(materialEnum type);
	void addMaterial(materialEnum type, UMaterialInterface *material);

	//weapon attachments
	UClass *findBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment);
	void addBp(weaponEnum weapon, weaponAttachmentEnum weaponAttachment, UClass *uclass);


	//ui
	UClass *uiBp();
	void addUiBp(UClass *uclassin);

	UTexture2D *findTexture(textureEnum type);
	void addTexture(textureEnum type, UTexture2D *texture);




	void setDebugCubeBp(UClass *uclassin);
	UClass *debugCubeBp();


private:
	assetManager();
	static class assetManager *instancePointer;


	//all asset maps
	assetManagerGeneric<entityEnum, UClass> entityAssets;

	
	assetManagerGeneric<weaponEnum, UClass> weaponAssets;
	assetManagerGeneric<throwableEnum, UClass> throwableAssets;
	assetManagerGeneric<particleEnum, UClass> particleAssets;

	assetManagerGeneric<materialEnum, UMaterialInterface> materialAssets;
	assetManagerGeneric<textureEnum, UTexture2D> textureAssets;

	std::map<weaponEnum, assetManagerGeneric<weaponAttachmentEnum, UClass>> weaponAttachmentAssets;

	UClass *uiBpPointer = nullptr;

	UClass *debugCubePointer = nullptr;
};
