// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutPost/OutpostManager.h"
#include "EGameState.h"
#include "p2/_world/gameStateManager/GameStateManager.h"
#include "p2/_world/PlayerStatManager/PlayerStatusManager.h"
#include "GameCore/world/worldLevelBase.h"

#include "p2/ui/_uiSimulation/UiSimulation.h"

#include "worldLevel.generated.h"

class ATerrainLauncher;

/**
 * saves the world data and nesecarry instance pointers 
 * - like the entity manager for reusing entities
 * - outpost manager to group enteties in the area
 * - terrain creator to create the terrain and keep 
 */
UCLASS()
class P2_API AworldLevel : public AworldLevelBase
{
	GENERATED_BODY()


protected:
	virtual void OnPlayerReferenceSet() override;

private:
	

	ATerrainLauncher *terrainLauncher = nullptr;

	bool bBlockEntities = false; //true (was false for debugging)
	bool bBlockOutPostCreation = true;

	//static AworldLevel *InstanceWorldLevel;

public:
	//call to init the world.
	static void initWorld(UWorld *world);
	static void initWorld(FString worldName);
	void EndCurrentWorld();

	static void MakeInstance(UWorld *world);

	AworldLevel();

	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);


	/// @brief for ui chnages, open loading screen, pause, etc
	static GameStateManager gameStateManager;

	/// for player hud changes
	static PlayerStatusManager playerStatusManager;
	static UiSimulation uiSimulation;

	static EntityManager *entityManager();
	static OutpostManager *outpostManager();

	

	static AworldLevel *GetInstance(){
		if(Instance){
			return Cast<AworldLevel>(Instance);
		}
		return nullptr;
	}

private:
	
	void debugStoragePlugin();

	void createTerrain(FString worldName);
	


	void createOutpostsRequested();

	//pathfinder init edge collection
	void createPathFinder(FString worldName);

	void humanBotsOnStart(int count);

	//manager
	class OutpostManager *outpostManagerPointer = nullptr;

	//terrain
	bool isTerrainInited;

	//bots
	bool areBotsInited;
	bool nodesWereShown;


	void DebugCreateRooms();

	void DebugCreatedoor();


	void createGroundPane();

	void debugCreateWater();

	void debugCreateRock();

	void debugCreateWingsuitMesh();

	void debugMatrix();

	

	void debugBezier();

	void createAeroActor();

	void createCar();

	void createBoneActorDebug();

	void createPluckerBoneActorDebug();
};
