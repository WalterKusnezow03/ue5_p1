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
#include "p2/particleSystem/particle.h"
#include "p2/meshgen/customMeshActor.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "p2/meshgen/water/customWaterActor.h"
#include "p2/entityManager/EntityManagerGenericMap.h"
#include "p2/entityManager/EntityManagerGeneric.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/rooms/roomtypeEnum.h"


/**
 * OBJECT POOL MANAGER
 * 
 * SINGLE TON INSTANCE POINTER IS PROVIDED BY WORLD LEVEL CLASS 
 * 
 * will spawn and hold entites which are despawned
 * only this class will spawn any enteties and manage them if they are not needed!
 * will help with object pooling for the whole game
 */
class P2_API EntityManager
{
private:
	FCollisionQueryParams collisionIgnoreParams;
	

	std::map<teamEnum, FCollisionQueryParams> collisionMap;

public:
	void addActorToIgnoreRaycastParams(AActor *actor, teamEnum team);
	void addActorToIgnoredAllParams(AActor *actor);

	FCollisionQueryParams &getIgnoredRaycastParams();
	FCollisionQueryParams &getIgnoredRaycastParams(teamEnum team);

	EntityManager();
	~EntityManager();

	//add entity section
	void add(AEntityScript *entity);
	void add(AHumanEntityScript *humanEntity);
	void add(Aweapon *weaponIn);
	void add(AthrowableItem *throwableItem);
	void add(Aparticle *particleIn);
	void add(AcustomMeshActor *meshActorIn);
	

	//spawn section
	AHumanEntityScript *spawnHumanEntity(UWorld *world, FVector &Location, teamEnum team);
	AEntityScript *spawnEntity(UWorld *world, FVector &Location);

	Aweapon *spawnAweapon(UWorld *world, weaponEnum typeToSpawn); //no attachments, only spawn by type
	Aweapon *spawnAweapon(UWorld *world, weaponSetupHelper *helper); //spawn with helper object for attachments
	//spawn a thrower weapon
	Aweapon *spawnAweapon(UWorld *world, throwableEnum throwableType);

	AthrowableItem *spawnAthrowable(UWorld *world, FVector &location, throwableEnum type); //spawns a throwable if existent

	AcustomMeshActor *spawnAcustomMeshActor(UWorld *world, FVector &location);

	

	/// @brief spawns aactor in the world
	/// @param world world to spawn in, cannot be nullptr!
	/// @param toSpawn class / bp to spawn
	/// @param Location location to spawn at 
	/// @return pointer to AActor
	AActor *spawnAactor(UWorld *world, UClass *toSpawn, FVector &Location);

	
	//particles
	void createExplosion(UWorld *world, FVector &location);
	void createFire(UWorld *world, FVector &location);
	void createDebree(UWorld *world, FVector &location, materialEnum materialType);


	//terrain
	std::vector<AcustomMeshActor *> requestMeshActors(UWorld *world, int requestCount);

	//meshes in general
	void createTwoSidedQuad(UWorld *world, FVector &a, FVector &b, FVector &c, FVector &d);

private:
	

	//empty mesh actor
	UPROPERTY()
	class UClass *emptyCustomMeshActorBp = nullptr;


	//hier nur den typ parameter als klasse angeben nicht pointer oderso
	//weil hier so nicht gewollt, wie in java den generic type
	//team unabhängig speichern
	EntityManagerGeneric<AEntityScript> entityList; 
	
	//hier werden ALLE toten humans ausbewahrt, basierend auf team
	EntityManagerGenericMap<teamEnum, AHumanEntityScript> humanEntityMap;
	EntityManagerGeneric<AcustomMeshActor> meshActorList;

	//GENERIC ENUM MAPS OF GENERIC MANAGERS / BACKUP ON DEATH 
	EntityManagerGenericMap<weaponEnum, Aweapon> weaponMap;
	EntityManagerGenericMap<throwableEnum, AthrowableItem> throwableMap;
	EntityManagerGenericMap<particleEnum,Aparticle> particleMap;

	

	//Particles 

	/// @brief returns a pointer (which should be dereferenced for a set for the blueprint)
	/// or used to instantiate the correct particle
	/// @param type type to get from this iv variables
	/// @return uclass or nullptr 
	UClass *getParticleBp(particleEnum type);

	void createParticle(UWorld *world, particleEnum enumtype, FVector &location, FVector &dir, float speed, float lifeTime);
	void createParticle(
		UWorld *world,
		UMaterial *materialToApply, 
		FVector &location,
		FVector &dir,
		float speed,
		float lifeTime
	);
};
