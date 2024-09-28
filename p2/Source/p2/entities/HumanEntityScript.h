// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "EntityScript.h"
#include "p2/weapon/weapon.h"
#include "skelletonController.h"
#include "HumanEntityScript.generated.h"


//forward declaration
class AOutpost;

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
	virtual void init() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void die() override;

private:
	//weapon
	class Aweapon *weaponPointer;

	void attackPlayer();
	void shootAt(FVector target);

	//outpost
	class AOutpost *outpost;
	void findOutPostNearby();

	//range of outpost check
	virtual bool isWithinMaxRange(FVector vec) override;
	
	//pointer for the skelleton, for now is just in human, not entity
	//class AskelletonController *skelletonControllerPointer = nullptr;

public:
	void setOutpost(AOutpost *outpostIn);

	virtual void despawn() override;

	
};
