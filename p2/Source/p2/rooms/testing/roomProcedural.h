// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActor.h"
#include "GameFramework/Actor.h"
#include "p2/util/TTouple.h"
#include "p2/rooms/testing/roomBoundData.h"
#include "roomProcedural.generated.h"

UCLASS()
class P2_API AroomProcedural : public AcustomMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AroomProcedural();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static void generate(UWorld *world, int sizeXMeters, int sizeYMeters, FVector location);

	

private:
	MeshData createWall(
		FVector from,
		FVector to,
		std::vector<FVector> &doors,
		std::vector<FVector> &windows,
		int doorWidthCm,
		int scaleZCm,
		FVector &locationOffset,
		FVector &centerOfRoom
	);

	void filterForVectorsBetween(
		FVector &A,
		FVector &B,
		int minDistance,
		std::vector<FVector> &positionsToFilter,
		std::vector<FVector> &output
	);

	void sortVectorsBetween(FVector &a, FVector &b, std::vector<FVector> &vec);

	float lenghtOf(FVector &vec);

	void spawnWindowMeshFromBounds(
		std::vector<TTouple<FVector, FVector>> &windowTouples,
		FVector &offset
	);







	void createRoom(
		FVector location,
		roomBoundData &someData,
		int oneMeter
	);

	void createRoom(
		FVector &location, // bottom left corner
		int scaleMetersX,
		int scaleMetersY,
		int scaleMetersZ,
		std::vector<FVector> &doorPositions,
		std::vector<FVector> &windowPositions,
		int onemeter
	);

	static AroomProcedural *spawnRoom(UWorld *world, FVector location);
	static void spawnRooms(UWorld *world, FVector location, std::vector<roomBoundData> &vec);
};
