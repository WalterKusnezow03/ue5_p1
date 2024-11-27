// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API TargetInterpolator
{
public:
	TargetInterpolator();
	~TargetInterpolator();

	void setTarget(FVector from, FVector toTarget, float timeToFrameIn);
	void overrideTarget(FVector totarget);
	void overrideStart(FVector fromTarget);


	bool hasReachedTarget();
	bool hasTargetSetup();

	float TimeToFrame();
	void resetDeltaTime();

	FVector interpolate(float DeltaTime);
	static FVector interpolation(FVector from, FVector to, float skalar);

	FVector readFromPosition();
	FVector readToPosition();

private:
	FVector from;
	FVector target;

	float deltaTime = 0.0f;
	float timeToFrame = 0.0f;
	bool reached = false;
	float skalar();

	bool targetSetup = false;
};
