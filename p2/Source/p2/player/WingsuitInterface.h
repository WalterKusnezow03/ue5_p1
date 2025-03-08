// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/util/timer.h"

/**
 * 
 */
class P2_API WingsuitInterface
{
public:
	WingsuitInterface();
	~WingsuitInterface();

	void Tick(
		UWorld *worldin,
		AActor &actor, 
		float DeltaTime
	);

	void setWingsuitTimerOnJump();
	void setWingsuitTimerOnMovement();

	float currentGravityMultiplier();
	bool wingsuitIsOpenFlag();

private:
	AActor *actorPointer = nullptr;
	UWorld *world = nullptr;

	float gravityDownAcceleration = -980.0f;
	
	bool wingsuitIsOpen = false;
	float wingsuitUpdateInvertall = 1.0f;
	class timer wingsuitTimer;
	int minDistanceGroundForWingsuit = 200; //10000

	FVector actorLocation;

	void setWingsuitTimer(float time);
	void TickWingsuitTimer(float DeltaTime);
	bool isInAirRaycast(FVector Start, float &distanceFromGround);
	void tryOpenWingsuit();
	void changeGravityDefault();
	void changeGravityWingSuit();

	float gravityCmsDown();
};
