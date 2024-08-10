// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityScript.h"
#include "weapon.h"
#include "HumanEntityScript.generated.h"

/**
 * Subclass of entity: added functionality for outpost and 
 * attacking the player with their weapon
 */
UCLASS()
class P2_API AHumanEntityScript : public AEntityScript
{
	GENERATED_BODY()
	
public:
	AHumanEntityScript();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class Aweapon *weaponPointer;

	void attackPlayer();
	void shootAt(FVector target);
};
