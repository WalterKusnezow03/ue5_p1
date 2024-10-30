// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/entities/customIk/BoneIk.h"
#include "IkActor.generated.h"

UCLASS()
class P2_API AIkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIkActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

private:
	float legScaleMeters = 2.0f;

	//depreacted sort of
	float velocity = 1.0f;
	float halfVelocity = 0.0f; //for offset time later, aber eins nach dem anderen.

	/// @brief degree per second constant for etha moving
	float degreePerSecond = 40;

	//legs for now
	class BoneIk leg1;
	class BoneIk leg2;
	class BoneIk arm1;

	void updateBone(BoneIk &bone, float deltaTime, FColor color);

	FVector offset; //global offset
	MMatrix ownLocation;
	void updatePositionBasedOnMovedDistance(BoneIk &boneToTrack);

	void LookAt(FVector TargetLocation);

	//testing needed and refacturing:
	MMatrix ownOrientation;


	//new testing more arm targets
	FVector targetA;
	FVector targetB;
	float timeCopy;
	int direction = 1;
	void debugDynamicArmTick(float DeltaTime);

};
