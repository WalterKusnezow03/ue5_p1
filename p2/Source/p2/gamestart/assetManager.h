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

private:
	assetManager();
	static class assetManager *instancePointer;


	//all asset maps
	class assetManagerGeneric<roomAssetEnum, UClass> roomAssets;
};
