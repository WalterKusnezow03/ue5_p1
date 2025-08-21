// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "terrainHillSetup.h"
#include <set>
#include "terrainPlugin/meshgen/generation/helper/TerrainChunkSetup.h"
#include "GameCore/MeshGenBase/foliage/ETerrainType.h"
#include "GameCore/util/FVectorTouple.h"
#include "chunk.h"
#include "GameCore/util/TVector.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/RoadMaker.h"


#include "terrainPlugin/meshgen/generation/TerrainCreator/ChunkSetup/TerrainChunkMap.h"
#include "terrainPlugin/main/worldCache/ChunkParserMap.h"

/**
 * 
 */
class TERRAINPLUGIN_API terrainCreator
{
public:
	terrainCreator();
	~terrainCreator();

	void debugCreateTerrain(UWorld *world);
	void debugCreateTerrain(UWorld *world, int meters);

	static const bool PLOTTING_ENABLED = false; // false;
	

	int chunkNum();
	
	void createTerrainAndSpawnMeshActors(UWorld *world, int meters);
	void createTerrainAndCreateBuildings(
		UWorld *world, int meters
	);
	

	//apply terrain
	void applyTerrainDataToMeshActors();

	//raycast
	float getHeightFor(FVector &position);
	float getHeightFor(FVector2D &pos);
	void getHeightAndDistanceFromModVertex(
		FVector2D &a,
		float &height,
		float &distance
	);

	void plotAllChunks(UWorld *world);


	//create actors
	AcustomMeshActor *getNewMeshActor();


	//new mesh data copying
	void createTerrainAndSetupChunkParserMap(
		ChunkParserMap &mapToFillDataTo,
		int chunksAxis
	);
	void applyTerrainDataIntoChunkParserMapCache(
		ChunkParserMap &mapToFillDataTo
	);

private:
	void applyTerrainDataIntoChunkParserAt(ChunkParserMap &mapToFillDataTo, int x, int y);

	void setFlatArea(FVector &location, int sizeMetersX, int sizeMetersY);

	void applyTerrainDataToMeshActors(
		int lowerX,
		int xLimit,
		int lowerY,
		int yLimit
	);
	void createWaterPaneAt(FVector &location);

	void markCreateOutpostsAt(
		std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea
	);



	void createTerrain(UWorld *world, int meters);
	void createTerrain(
		UWorld *world,
		int meters,
		std::vector<terrainHillSetup> &predefinedHillDataVecFlatArea // flat area
	);

	

	

	class UWorld *worldPointer = nullptr;

	std::vector<std::vector<chunk>> map;

	
	
	
	
	void smooth3dMap();
	void smooth3dMap(FVector &a, FVector &b, int iterations);

	void applyColumnOrRow(
		int index,
		TVector<FVector2D> &data,
		bool isColumn
	);

	bool verifyIndex(int a);
	int clampIndex(int a);

public:
	//converts a cm value to a chunk index.
	int cmToChunkIndex(int a);

private:
	int cmToMeter(int a);
	int meterToInnerChunkIndex(int a);
	int cmToInnerChunkIndex(int a);


	void scaleHeightForAll(float scale);




	void flattenChunksForHillData(std::vector<terrainHillSetup> &hillDataVec);
	void flattenChunksForHillData(terrainHillSetup &hillData);

	void createChunkAtIfNotCreatedYet(int x, int y);

	//--- terrain type apply helpers ---
	void randomizeTerrainTypes(UWorld *world);
	void applyTerrainTypeBetween(FVector &a, FVector &b, ETerrainType typeIn);
	
public:	
	chunk *chunkAt(int x, int y);
	chunk *chunkAt(terrainHillSetup &setup);
	TArray<chunk *> chunksAt(
		TArray<FVector> &positionsWorld
	);

private:
	std::vector<ETerrainType> createRandomTerrainTypes(int count);
	ETerrainType selectTerrainTypeExcluding(ETerrainType typeToExclude);

	void applySpecialTerrainTypesByHeight();


	//--- flat outpost helper ---
	void createFlatAreas(
		int count,
		int minsizeChunks,
		int maxsizeChunks,
		int chunkRange,
		std::vector<terrainHillSetup> &output
	);
	void createFlatArea(
		int minsizeChunks,
		int maxsizeChunks,
		int chunkRange,
		std::vector<terrainHillSetup> &output
	);

	void findChunksEnclosedBy(
		std::vector<terrainHillSetup> &hills,
		std::vector<chunk *> &output
	);
	void findChunksEnclosedBy(
		terrainHillSetup &hillData,
		std::set<chunk *> &output
	);


	void createRoads(UWorld* world);
	RoadMaker roadmaker;




	//NEW
	TerrainChunkMap setupMap;
	void setupFromChunkMap(TerrainChunkMap &ref, int x, int y, int numChunksSide);
};
