// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "room.generated.h"

UCLASS()
class P2_API Aroom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aroom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void processDoorPositionVectors(std::vector<FVector> &toPositionVector);

private:
	void debugShowOutline();
	void findDoors();
	void calculateActorBounds();
	FVector bottomLeftCorner();
	std::vector<FVector> debugAllCorners();

	std::vector<FVector> doorPositions;

	FVector boxOrigin;
	FVector boxExtent;
	int boxXScale;
	int boxYScale;
	int boxZScale;
};
