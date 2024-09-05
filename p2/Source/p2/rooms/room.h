// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "roomtypeEnum.h"
#include "room.generated.h"

UCLASS()
class P2_API Aroom : public AActor
{
	GENERATED_BODY()
	
public:
	static const bool DEBUG_DRAW = false;

	// Sets default values for this actor's properties
	Aroom();

	roomtypeEnum readType();

protected:
	//blue print var for defining the type of the room properly
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="room type")
	roomtypeEnum type;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void processDoorPositionVectors(std::vector<FVector> &toPositionVector, UClass *doorBp);

private:
	


	void debugShowOutline();
	void findDoors();
	void calculateActorBounds();
	FVector bottomLeftCorner();
	std::vector<FVector> debugAllCorners();

	std::vector<FVector> doorPositions;
	TArray<AActor *> wallActors;
	TArray<UChildActorComponent *> walls;

	void findWalls();
	void disableWall(FVector &location);
	void disableWall(FVector &location, UClass *doorToSpawn);

	FVector boxOrigin;
	FVector boxExtent;
	int boxXScale;
	int boxYScale;
	int boxZScale;
};
