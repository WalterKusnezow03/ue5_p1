// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "customMeshActorBase.h"
#include "customMeshActorWater.generated.h"

/**
 * 
 */
UCLASS()
class P2_API AcustomMeshActorWater : public AcustomMeshActorBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void createPane(int sizeX, int sizeY, int detail);

private:
	void TickWaterFunction(MeshData *data, float DeltaTime);
	void refreshMesh();

	int LAYER = 0;
};
