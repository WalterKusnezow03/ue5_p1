// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityManagerGeneric.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/throwableItems/throwableEnum.h"
#include "p2/throwableItems/throwableItem.h"


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

	Aweapon *spawnAweapon(UWorld *world, weaponEnum typeToSpawn); //default spawn by type
	Aweapon *spawnAweapon(UWorld *world, weaponSetupHelper *helper); //spawn with helper object for attachments

	AthrowableItem *spawnAthrowable(UWorld *world, FVector location, throwableEnum type); //spawns a throwable if existent

	void setEntityUClassBp(UClass *entityIn);
	void setHumanEntityUClassBp(UClass *humanIn);

	/// @brief sets the blueprint for all weapons
	/// @param weaponIn 
	/// @param type 
	void setWeaponUClassBP(UClass *weaponIn, weaponEnum type);

	//throwable setting
	void setThrowableUClassBp(UClass *throwableIn, throwableEnum type);

	//testing needed
	void setDefaultThrowerClassBp(UClass *uIn);

private:
	static class EntityManager *instancePointer;

	//deprecated
	Aweapon *spawnPistol(UWorld *world);

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

	UPROPERTY()
	class UClass *assaultRifleBpClass;


	//throwables section
	UPROPERTY()
	class UClass *grenadeBpClass;
	UPROPERTY()
	class UClass *molotovBpClass;
	UPROPERTY()
	class UClass *rocketBpClass;
	



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
	

	UPROPERTY()
	class UClass *defaultThrower;

	

	//testing needed
public:
	Aweapon *spawnAweapon(UWorld *world, throwableEnum throwableType);
};
