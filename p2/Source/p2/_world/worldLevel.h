// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/meshgen/generation/terrainCreator.h"

/**
 * saves the world data and nesecarry instance pointers 
 * - like the entity manager for reusing entities
 * - outpost manager to group enteties in the area
 * - terrain creator to create the terrain and keep 
 */
class P2_API worldLevel
{
public:
	static EntityManager *entityManager();
	static OutpostManager *outpostManager();
	
	static void resetWorld();

	static void initWorld(UWorld *world);

	static void createTerrain(UWorld *world, int meters);
	static int getGroundHeight(FVector &pos);

private:
	worldLevel();
	~worldLevel();

	//pathfinder init edge collection
	static void createPathFinder(UWorld *WorldIn);

	static void humanBotsOnStart(UWorld *worldIn, int count);

	//manager
	static class EntityManager *entityManagerPointer;
	static class OutpostManager *outpostManagerPointer;

	//terrain
	static class terrainCreator *terrainPointer;
	static bool isTerrainInited;

	//bots
	static bool areBotsInited;

	static void DebugCreateRooms(UWorld *world);

	static void debugBezier(UWorld *world);

	static void debugAngleFinder(UWorld *world);
	static std::vector<FVector2D> findAngles(float lengthAll, std::vector<float> &bones);


	static void debugCreateWater(UWorld *world);

};
