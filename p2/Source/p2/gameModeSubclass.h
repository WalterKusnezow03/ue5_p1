// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "p2/entityManager/EntityManager.h"
#include "gameModeSubclass.generated.h"

/**
 * 
 */
UCLASS()
class P2_API AgameModeSubclass : public AGameModeBase
{
	GENERATED_BODY()
public:
	AgameModeSubclass();
	void BeginPlay();

private:
	//loads a uclass from a specific path
	UClass* loadUClassBluePrint(FString path);
	UMaterial *loadMaterial(FString path);

	void createPathFinder();




	void loadWeapons(EntityManager *entityManager);
	void loadEntities(EntityManager *entityManager);
	void loadThrower(EntityManager *entityManager);
	void loadParticles(EntityManager *entityManager);
	void loadRooms(EntityManager *entityManager);
	void loadTerrain(EntityManager *e);





	//new testing
	void loadPaths();
	void loadUClassFromAssetPath(FString s);

};
