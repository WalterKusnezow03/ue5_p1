// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "assetManagerGeneric.h"
#include <map>

/**
 * 
 */
class P2_API assetManager
{
public:
	static assetManager *instance();
	~assetManager();

	//all asset map get methods
	UClass *findBp(roomAssetEnum type);
	void addBp(roomAssetEnum type, UClass *asset);

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
	UMaterial *findMaterial(materialEnum type);
	void addMaterial(materialEnum type, UMaterial *material);

private:
	assetManager();
	static class assetManager *instancePointer;


	//all asset maps
	class assetManagerGeneric<entityEnum, UClass> entityAssets;

	class assetManagerGeneric<roomAssetEnum, UClass> roomAssets;
	class assetManagerGeneric<weaponEnum, UClass> weaponAssets;
	class assetManagerGeneric<throwableEnum, UClass> throwableAssets;
	class assetManagerGeneric<particleEnum, UClass> particleAssets;

	class assetManagerGeneric<materialEnum, UMaterial> materialAssets;
};
