// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutPost/OutpostManager.h"
#include "EGameState.h"
#include "p2/_world/gameStateManager/GameStateManager.h"
#include "p2/_world/PlayerStatManager/PlayerStatusManager.h"
#include "GameCore/world/worldLevelBase.h"

class ATerrainLauncher;

/**
 * saves the world data and nesecarry instance pointers 
 * - like the entity manager for reusing entities
 * - outpost manager to group enteties in the area
 * - terrain creator to create the terrain and keep 
 */
class P2_API worldLevel : public worldLevelBase
{
private:
	worldLevel();
	~worldLevel();

	
	static ATerrainLauncher *terrainLauncher;

public:
	//call to init the world.
	static void initWorld(UWorld *world);
	static void initWorld(FString worldName);

	//api for game launch screen / game mode base
	static void clearGameSession();
	

	static EntityManager *entityManager();
	static OutpostManager *outpostManager();
	
	static void resetWorld();

	static void Tick(float DeltaTime);

	static bool terrainIsInitedFlag(){
		return isTerrainInited;
	}

	static bool gamePausedByPlayer();
	static void setGamePaused(bool in);

	/// @brief for ui chnages, open loading screen, pause, etc
	static GameStateManager gameStateManager;

	/// for player hud changes
	static PlayerStatusManager playerStatusManager;

private:
	
	static void debugStoragePlugin();

	static void createTerrain(UWorld *world, FString worldName);
	static UWorld *GetWorld();

	

	static void createOutpostsRequested(UWorld *world);

	//pathfinder init edge collection
	static void createPathFinder(UWorld *WorldIn, FString worldName);

	static void humanBotsOnStart(UWorld *worldIn, int count);

	//manager
	static class OutpostManager *outpostManagerPointer;

	//terrain
	static bool isTerrainInited;

	//bots
	static bool areBotsInited;
	static bool nodesWereShown;
	static bool gamePausedFlag;


	static void DebugCreateRooms(UWorld *world);

	static void DebugCreatedoor(UWorld *world);

	static void debugAngleFinder(UWorld *world);
	static std::vector<FVector2D> findAngles(float lengthAll, std::vector<float> &bones);

	static void createGroundPane(UWorld *world);

	static void debugCreateWater(UWorld *world);

	static void debugCreateRock(UWorld *world);

	static void debugCreateWingsuitMesh(UWorld *world);

	static void debugMatrix();

	

	static void debugBezier(UWorld *world);

	static void createAeroActor(UWorld *world);

	static void createCar(UWorld *world);

	static void createBoneActorDebug(UWorld *world);
};
