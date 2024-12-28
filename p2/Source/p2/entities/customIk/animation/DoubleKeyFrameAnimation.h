// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetInterpolator.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/entities/customIk/bonePackage/BoneControllerStates.h"
#include "KeyFrameAnimation.h"

/**
 * will store 2 seperate key frame animations and tell the status if A or B
 */
class P2_API DoubleKeyFrameAnimation
{
public:
	DoubleKeyFrameAnimation();
	~DoubleKeyFrameAnimation();

	bool isAnimationA();
	bool isAnimationB();

	void setAnimationA(KeyFrameAnimation &&A);
	void setAnimationBAdjustPermanentTarget(FVector vector);

	FVector interpolate(float DeltaTime);
	FVector interpolate(float DeltaTime, FVector currentRelative); //experimental
	FVector interpolateWorld(
		float DeltaTime,
		FVector currentPosWorld,
		MMatrix &actor
	);

	bool animationCycleWasComplete();

	void processProjectOffset(FVector &offsetMade);
	FVector getProjectionHipOffsetTimed(float DeltaTime);

	
	void overrideCurrentStartingFrame(FVector &currentLocationRelative);
	void skipAnimationOnce(FVector start, FVector end);
	void skipAnimationOnceWorld(MMatrix &actor, FVector start, FVector end);
	
	void projectNextFrameIfNeeded(UWorld *world, MMatrix &actorMatrix);
	
	void projectNextFrameIfNeeded(
		UWorld *world,
		MMatrix actorMatrix, // is value pass on purpose
		float velocity,
		FVector &lookdir,
		bool &switchToArmLocomotion,
		float maxHeightSwitch,
		BoneControllerStates locomotionType
	);

	void forceProjectNextFrame(
		UWorld *world,
		MMatrix &actorMatrix
	);

	void forceOverrideNextFrame(FVector &pos);

private:
	float animationSpeedHipAdjust = 50.0f;

	FVector projectionHipOffsetComplete;
	FVector bTarget;
	bool bIsSetToAutoOverride = false;

	//new velocity for anim B
	float velocityOfActor = 0.0f;

	bool reachedTime(float timeCheck);

	class KeyFrameAnimation framesA;
	
	class TargetInterpolator interpolateB;
	//class Interpolator interpolateB;

	bool isAnimationAPlaying = true;
	float deltaTime = 0.0f;

	bool cycleComplete = false;
	bool blocked = false;

	

	

	
	float reachTime();

	FVector aReachedTickFrame;

	float sinusFlyingOffset(float time, float width);

	bool isRunningAnimation = false;

	FVector flyingOffset();



	//neu testing
	FVector latestWorldProjectedFrame;


public:
	void setRunning(bool b);


	
};
