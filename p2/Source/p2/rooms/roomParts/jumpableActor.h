// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/util/TTouple.h"
#include "p2/player/playerScript.h"
#include "jumpableActor.generated.h"

UCLASS()
class P2_API AjumpableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AjumpableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//not purely virtual but designed to be overriden if wanted
	virtual void jumpOver(AActor *actor);
	virtual bool inRange(FVector &pos);

	void init();

protected:
	class AplayerScript *playerPointer = nullptr;

	static const int metersPerSecond = 5;
	static const int MAX_DISTANCE = 100;
	bool hasReached(FVector &a, FVector &b);

	void findTargetLocations();
	std::vector<FVector> targets;

	FVector& findFurthestTarget(FVector &asked);

	std::vector<TTouple<AActor*, FVector>> moving;

	void move(float DeltaTime, AActor *a, FVector &target);
};
