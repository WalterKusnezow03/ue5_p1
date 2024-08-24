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
	UClass* loadUClassBluePrint(FString path);

	void createPathFinder();
	void showEdges(std::vector<FVector> &edges);
	void showPath(std::vector<FVector> &path);
	void showPos(FVector &v, FColor color);





	void loadWeapons(EntityManager *entityManager);
	void loadEntities(EntityManager *entityManager);
	void loadThrower(EntityManager *entityManager);
	void loadParticles(EntityManager *entityManager);
	void loadRooms(EntityManager *entityManager);


	
};
