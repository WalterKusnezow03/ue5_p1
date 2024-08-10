// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageinterface.h"
#include "playerScript.h"

#include "EntityScript.generated.h"

UCLASS()
class P2_API AEntityScript : public AActor, public IDamageinterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntityScript();
	//inherited from interface
	virtual void takedamage(int d) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool performRaycast(AActor *target);

	
	void setSpottingTime(float time);
	void updateSpottingTime(float deltaTime);

	void LookAt(FVector TargetLocation);
	void LookAt(AActor *target);
	bool withinVisionAngle(AActor *target);

	//path
	std::vector<FVector> path;
	void moveTowardsPlayer(float deltaTime, bool canSeePlayer);
	void resetpath();
	void followpath(float deltaTime);
	bool hasNodesInPathLeft();
	bool reachedPosition(FVector pos);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int health;
	float defaultSpottingTime;
	float spottingTimeLeft;

	bool spottedPlayer;

	class AplayerScript* playerPointer;

	void die();

	void showScreenMessage(FString s);
};
