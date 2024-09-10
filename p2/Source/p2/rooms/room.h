// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/util/TTouple.h"
#include "roomtypeEnum.h"
#include "room.generated.h"

UCLASS()
class P2_API Aroom : public AActor
{
	GENERATED_BODY()
	
public:
	static const bool DEBUG_DRAW = true;

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

	//spawn walls on start
	void spawnWalls(UClass *bp);

	//replace walls with doors and windows etc
	void processPositionVectorsAndReplaceWall(std::vector<FVector> &toPositionVector, UClass *bp);

	int getXScale();
	int getYScale();
	int getZScale();

private:
	


	void debugShowOutline();
	
	void calculateActorBounds();
	FVector bottomLeftCorner();
	std::vector<FVector> debugAllCorners();
	std::vector<FVector> allCorners();

	std::vector<FVector> doorPositions;
	TArray<AActor *> wallActors;
	TArray<AActor *> disabledWallActors;


	void disableWall(FVector &location);
	void disableWall(FVector &location, UClass *bpToSpawn);

	//new
	void sortIn(
		std::vector<TTouple<float, AActor*>> &vec, // by reference, nicht vergessen!
		AActor* a,
		float distance,
		int sizetargeted
	);

	FVector boxOrigin;
	FVector boxExtent;
	int boxXScale;
	int boxYScale;
	int boxZScale;
};
