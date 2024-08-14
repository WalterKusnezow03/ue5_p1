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

	//add entity section
	void add(AEntityScript *entity);
	void add(AHumanEntityScript *humanEntity);
	void add(Aweapon *weaponIn);

	//spawn section
	AHumanEntityScript *spawnHumanEntity(UWorld *world, FVector Location);
	AEntityScript *spawnEntity(UWorld *world, FVector Location);

	Aweapon *spawnAweapon(UWorld *world);
	Aweapon *spawnPistol(UWorld *world);

	void setEntityUClassBp(UClass *entityIn);
	void setHumanEntityUClassBp(UClass *humanIn);
	void setWeaponUClassBp(UClass *weaponIn);
	void setPistolUClassBp(UClass *pistolIn);

private:
	static class EntityManager *instancePointer;

	EntityManager();

	//bp classes USE "UPROPERTY()" TO SAFELY USE UCLASS POINTER!
	UPROPERTY()
	class UClass *entityBpClass;

	UPROPERTY()
	class UClass *humanEntityBpClass;

	UPROPERTY()
	class UClass *weaponBpClass;

	UPROPERTY()
	class UClass *pistolBpClass;


	//hier nur den typ parameter als klasse angeben nicht pointer oderso
	//weil hier so nicht gewollt, wie in java
	class EntityManagerGeneric<AEntityScript> entityList; 
	class EntityManagerGeneric<AHumanEntityScript> humanEntityList;

	//weapons (each type has an own manager)
	class EntityManagerGeneric<Aweapon> assault_weaponList;
	class EntityManagerGeneric<Aweapon> pistol_weaponList;




	/// @brief spawns aactor in the world
	/// @param world world to spawn in, cannot be nullptr!
	/// @param toSpawn class / bp to spawn
	/// @param Location location to spawn at 
	/// @return 
	AActor *spawnAactor(UWorld *world, UClass *toSpawn, FVector Location);
	

	
};
