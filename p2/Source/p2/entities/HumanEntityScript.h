// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "EntityScript.h"
#include "p2/weapon/weapon.h"
#include "p2/entities/botActionHelper/EAttackType.h"

#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"
#include "PathfinderNNExtension/Interface/VisionCone.h"

#include "HumanEntityScript.generated.h"


//forward declaration
class AOutpost;

/**
 * Subclass of entity: added functionality for outpost and 
 * attacking the player with their weapon
 */
UCLASS()
class P2_API AHumanEntityScript : public AEntityScript, public IPathfinderNNInterface
{
	GENERATED_BODY()

private:
	bool bDebugBlockOutpostRequest = true;

public:
	AHumanEntityScript();
	virtual void init() override;
	virtual void init(FVector &location) override;

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

	void PerformActionsBasedOnPlayerVisibility();
	void adaptWeaponToCurrentPlayerVisibilty();

	void attackPlayer();
	void shootAt(FVector target);

	//outpost
	class AOutpost *outpost;
	void findOutPostNearby();

	//range of outpost check
	virtual bool isWithinMaxRange(FVector vec) override;

	void reloadOwnWeaponIfNeeded();

	//bot extra actions
	EAttackType attackTypeOfBot = EAttackType::EAssault;
	bool attackTypeIs(EAttackType type);

public:
	void setOutpost(AOutpost *outpostIn);
	void clearOutpost();

	virtual void despawn() override;

	using Super::alert; // <- macht alert() aus der Basisklasse wieder sichtbar
	virtual void alert(FVector lookat) override;

	virtual FVisionCone &GetVisionCone() override;

protected:
	virtual void requestNewPathTo(FVector &location, bool towardsPlayer) override;


	
	bool playerIsInLookDir();


	//-- death payload / widget interaction override --
	virtual void SetupLootDeathPayload() override; //is called on widget request.

	FVisionCone visionCone;

	// NN Interface
	void FlagPlayerVisibleToNNInterface();
	void FlagActorVisibleToNNInterface(AActor *actor);

	void RequestPlayerPredictionFromNNInterface();

public:
	virtual void ResponseNNPositions(const TArray<FVector> &positions) override;
	virtual FVector GetWorldLocation() override {
		return GetActorLocation();
	}
	// NN Interface
};
