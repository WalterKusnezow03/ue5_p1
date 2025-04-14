// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API GravityInterpolator
{
public:
	GravityInterpolator();
	~GravityInterpolator();

	FVector interpolate(FVector &currentPos, float DeltaTime);
	FVector interpolate(FVector &currentPos, FVector &velocity, float DeltaTime);
	void resetVelocity();

	void updateGroundPosition(FVector &groundIn);
	void updateGroundPositionAndSetInitialVelocity(FVector &groundIn, FVector &initVelocity);
	bool groundReachedFlag();

	FVector copyGroundPosition();

	void setGravity(float gravityin);

	float skalar(FVector &currentPos);

private:
	FVector groundPosition;
	float epsilonGroundReached = 0.5f;

	bool reachedFlag = true;

	float DeltaTimeSum = 0.0f;

	FVector gravityVector();
	bool groundReached(FVector &compare);
	float gravityCmsDown();

	float gravityA = -981.0f; //9.81 meter
	FVector velocityVector;

	float timeUntilGroundContact(FVector &currentPos);
};
