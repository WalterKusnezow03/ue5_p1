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
	
	void createPathFinder();


};
