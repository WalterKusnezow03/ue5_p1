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
#include "p2/particleSystem/particleEnum.h"
#include "p2/rooms/room.h"
#include "p2/entityManager/RoomManager.h"


/**
 * OBJECT POOL MANAGER
 * 
 * will spawn and hold entites which are despawned
 * only this class will spawn any enteties and manage them if they are not needed!
 * will help with object pooling for the whole game
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
	void add(AthrowableItem *throwableItem);

	//spawn section
	AHumanEntityScript *spawnHumanEntity(UWorld *world, FVector &Location);
	AEntityScript *spawnEntity(UWorld *world, FVector &Location);

	Aweapon *spawnAweapon(UWorld *world, weaponEnum typeToSpawn); //no attachments, only spawn by type
	Aweapon *spawnAweapon(UWorld *world, weaponSetupHelper *helper); //spawn with helper object for attachments
	//spawn a thrower weapon
	Aweapon *spawnAweapon(UWorld *world, throwableEnum throwableType);

	AthrowableItem *spawnAthrowable(UWorld *world, FVector &location, throwableEnum type); //spawns a throwable if existent

	/// @brief spawns aactor in the world
	/// @param world world to spawn in, cannot be nullptr!
	/// @param toSpawn class / bp to spawn
	/// @param Location location to spawn at 
	/// @return pointer to AActor
	AActor *spawnAactor(UWorld *world, UClass *toSpawn, FVector &Location);

	//Set bp methods

	void setEntityUClassBp(UClass *entityIn);
	void setHumanEntityUClassBp(UClass *humanIn);

	/// @brief sets the blueprint for all weapons
	/// @param weaponIn 
	/// @param type 
	void setWeaponUClassBP(UClass *weaponIn, weaponEnum type);

	//throwable setting
	void setThrowableUClassBp(UClass *throwableIn, throwableEnum type);

	//thrower default (type is set upon instantiating request)
	void setDefaultThrowerClassBp(UClass *uIn);

	//particles
	void setparticleBp(UClass *uIn, particleEnum typeIn);

	void createExplosion(UWorld *world, FVector &location);



	//rooms
	void setRoomuClassBp(UWorld *world, UClass *uclass);
	Aroom *createRoom(UWorld *worldIn, FVector &location, int xScale, int yScale);

	//testing
	void createALayout(UWorld *worldIn, FVector &location, int x, int y);

private:
	static class EntityManager *instancePointer;

	//deprecated
	Aweapon *spawnPistol(UWorld *world);

	EntityManager();

	//bp classes USE "UPROPERTY()" TO SAFELY USE UCLASS POINTER!
	UPROPERTY()
	class UClass *entityBpClass = nullptr;

	UPROPERTY()
	class UClass *humanEntityBpClass = nullptr;

	UPROPERTY()
	class UClass *weaponBpClass = nullptr;

	UPROPERTY()
	class UClass *pistolBpClass = nullptr;

	UPROPERTY()
	class UClass *assaultRifleBpClass = nullptr;


	//throwables section
	UPROPERTY()
	class UClass *grenadeBpClass = nullptr;
	UPROPERTY()
	class UClass *molotovBpClass = nullptr;
	UPROPERTY()
	class UClass *rocketBpClass = nullptr;

	//thrower
	UPROPERTY()
	class UClass *defaultThrower = nullptr;
	

	//particle section
	UPROPERTY()
	class UClass *smokeParticleBp = nullptr;

	UPROPERTY()
	class UClass *fireParticleBp = nullptr;


	//room section
	UPROPERTY()
	class UClass *room1 = nullptr;
	class RoomManager roomType1Manager;







	//hier nur den typ parameter als klasse angeben nicht pointer oderso
	//weil hier so nicht gewollt, wie in java
	class EntityManagerGeneric<AEntityScript> entityList; 
	class EntityManagerGeneric<AHumanEntityScript> humanEntityList;

	//weapons (each type has an own manager)
	class EntityManagerGeneric<Aweapon> assault_weaponList;
	class EntityManagerGeneric<Aweapon> pistol_weaponList;

	EntityManagerGeneric<Aweapon> *getWeaponManagerFor(weaponEnum type);


	//throwables
	class EntityManagerGeneric<AthrowableItem> grenadeList;
	class EntityManagerGeneric<AthrowableItem> rocketList;
	class EntityManagerGeneric<AthrowableItem> molotovList;
	class EntityManagerGeneric<AthrowableItem> rockList;

	EntityManagerGeneric<AthrowableItem> *getThrowableManagerFor(throwableEnum type);

	




	//Particles 

	/// @brief returns a pointer (which should be dereferenced for a set for the blueprint)
	/// or used to instantiate the correct particle
	/// @param type type to get from this iv variables
	/// @return uclass or nullptr 
	UClass *getParticleBp(particleEnum type);

	void createParticle(UWorld *world, particleEnum enumtype, FVector &location, FVector &dir, float speed, float lifeTime);


};
