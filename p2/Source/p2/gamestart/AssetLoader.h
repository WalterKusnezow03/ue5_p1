// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * will load all nesecarry assets, bots, weapons etc
 * must be called from gamemode class constructor
 */
class P2_API AssetLoader
{
public:
	AssetLoader(UWorld *worldIn);
	~AssetLoader();

	void loadAssets();

private:
	class UWorld *world;

	//loads a uclass from a specific path
	UClass* loadUClassBluePrint(FString path);
	UMaterial *loadMaterial(FString path);

	FString buildPath(FString path, FString bluePrintName);
	void cleanUpPath(FString &s);

	void createPathFinder(UWorld *WorldIn);




	void loadWeapons(EntityManager *entityManager);
	void loadEntities(EntityManager *entityManager);
	void loadThrower(EntityManager *entityManager);
	void loadParticles(EntityManager *entityManager);
	void loadRooms(EntityManager *entityManager);
	void loadTerrain(EntityManager *e);
	void loadMaterials();

	//new testing
	void loadPaths();
	void loadUClassFromAssetPath(FString s);
};
