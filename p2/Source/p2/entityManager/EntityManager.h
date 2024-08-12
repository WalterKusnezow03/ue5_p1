// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityManagerGeneric.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"

/**
 * will spawn and hold entites which are despawned
 * only this class will spawn any enteties and manage them if they are not needed!
 */
class P2_API EntityManager
{
public:
	static EntityManager *instance();

	~EntityManager();

	void add(AEntityScript *entity);

	void add(AHumanEntityScript *humanEntity);

	
	AHumanEntityScript *spawnHumanEntity(UWorld *world, FVector Location);
	AEntityScript *spawnEntity(UWorld *world, FVector Location);

	Aweapon *spawnAweapon(UWorld *world);

	void setEntityUClassBp(UClass *entityIn);
	void setHumanEntityUClassBp(UClass *humanIn);
	void setWeaponUClassBp(UClass *weaponIn);

private:
	static class EntityManager *instancePointer;

	EntityManager();
	//hier nur den typ parameter als klasse angeben nicht pointer oderso
	//weil hier so nicht gewollt, wie in java
	class EntityManagerGeneric<AEntityScript> entityList; 
	class EntityManagerGeneric<AHumanEntityScript> humanEntityList; 

	/// @brief spawns aactor in the world
	/// @param world world to spawn in, cannot be nullptr!
	/// @param toSpawn class / bp to spawn
	/// @param Location location to spawn at 
	/// @return 
	AActor *spawnAactor(UWorld *world, UClass *toSpawn, FVector Location);
	

	class UClass *entityBpClass;
	class UClass *humanEntityBpClass;
	class UClass *weaponBpClass;

};
