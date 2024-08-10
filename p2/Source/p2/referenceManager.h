// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "EntityScript.h"
#include "playerScript.h"

/**
 * 
 */
class P2_API referenceManager
{
public:
	
	~referenceManager();

	static referenceManager *instance();

	void setPlayerReference(AplayerScript *newPointer);
	AplayerScript *getPlayerPointer();

	//set class bps
	void setEntityUClassBp(UClass *entityIn);
	void setWeaponUClassBp(UClass *weaponIn);

	//spawn methods
	void spawnEntity(UWorld *world, FVector location);


	Aweapon *spawnAweapon(UWorld *world);

private:
	//private constructor for singleton
	referenceManager();

	/// @brief spawns aactor in the world
	/// @param world world to spawn in, cannot be nullptr!
	/// @param toSpawn class / bp to spawn
	/// @param Location location to spawn at 
	/// @return 
	AActor *spawnAactor(UWorld *world, UClass *toSpawn, FVector Location);

	
	static class referenceManager *instancePointer;

	class AplayerScript *playerPointer;

	class UClass *entityBpClass;
	class UClass *weaponBpClass;
};
