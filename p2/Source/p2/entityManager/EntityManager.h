// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameCore/EntityGC/EntityManagerBase.h"

#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"

#include "p2/weapon/throwerWeapon.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "AssetEnumCollection/assetEnums/throwableEnum.h"
#include "p2/throwableItems/throwableItem.h"

#include "terrainPlugin/meshgen/customMeshActor.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "terrainPlugin/meshgen/water/customWaterActor.h"

#include "GameCore/EntityGC/EntityManagerGeneric.h"

#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "AssetEnumCollection/assetEnums/throwableEnum.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "AssetEnumCollection/assetEnums/entityEnum.h"

#include "p2/ui/3Dui/GamePlayWidgets/Enum/EWorldDynamicWidgetEnum.h"
#include "p2/vfx/type/EVFXActorType.h"


#include "GameCore/Raycast/query/mapTracker/TCollisionTracker.h"


class AWorldDynamicWidgetActor;
class AVFXActor;

/**
 * OBJECT POOL MANAGER
 * 
 * SINGLE TON INSTANCE POINTER IS PROVIDED BY WORLD LEVEL CLASS 
 * 
 * will spawn and hold entites which are despawned
 * only this class will spawn any enteties and manage them if they are not needed!
 * will help with object pooling for the whole game
 * 
 * communication to Gc / obejct pool Plugin
 */
class P2_API EntityManager : public EntityManagerBase
{
private:
	//deprecated
	//FCollisionQueryParams collisionIgnoreParams;
	//std::map<teamEnum, FCollisionQueryParams> collisionMap;

	TCollisionTracker<teamEnum> tCollisionMap;

public:

	static void BeginPlay();
	static EntityManager *instance();


	void addActorToIgnoreRaycastParams(AActor *actor, teamEnum team);
	void addActorToIgnoredAllParams(AActor *actor);

	FCollisionQueryParams getIgnoredRaycastParams();
	FCollisionQueryParams getIgnoredRaycastParams(teamEnum team);

	EntityManager();
	~EntityManager();

	//using base class
	using EntityManagerBase::add;

	//add entity section
	void add(AEntityScript *entity);
	void add(AHumanEntityScript *humanEntity);
	void add(Aweapon *weaponIn);
	void add(AthrowableItem *throwableItem);
	
	void add(AcustomMeshActor *meshActorIn);
	void add(AWorldDynamicWidgetActor *actorIn);
	void add(AVFXActor *actorIn);

	//spawn section
	AHumanEntityScript *spawnHumanEntity(UWorld *world, FVector &Location, teamEnum team);
	AEntityScript *spawnEntity(UWorld *world, FVector &Location);

	Aweapon *spawnAweapon(UWorld *world, weaponEnum typeToSpawn); //no attachments, only spawn by type
	Aweapon *spawnAweapon(UWorld *world, weaponSetupHelper *helper); //spawn with helper object for attachments
	//spawn a thrower weapon
	Aweapon *spawnAweapon(UWorld *world, throwableEnum throwableType);

	AthrowableItem *spawnAthrowable(UWorld *world, FVector &location, throwableEnum type); //spawns a throwable if existent

	AcustomMeshActor *spawnAcustomMeshActor(UWorld *world, FVector &location);


	AWorldDynamicWidgetActor *spawnAWorldDynamicWidgetActor(
		EWorldDynamicWidgetEnum typeWidgetActorToSpawn,
		UWorld *world, 
		USceneComponent *attachTo,
		FVector relativeLocation
	);

	AVFXActor *spawnAVFXActor(EVFXActorType typeVFX);

	/// @brief spawns aactor in the world
	/// @param world world to spawn in, cannot be nullptr!
	/// @param toSpawn class / bp to spawn
	/// @param Location location to spawn at 
	/// @return pointer to AActor
	AActor *spawnAactor(UWorld *world, UClass *toSpawn, FVector &Location);

	
	//particles
	void createExplosion(FVector &location);
	



private:
	

	

	

	
};
