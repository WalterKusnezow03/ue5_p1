// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetInterpolator.h"
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

	//From KeyFrame Class
	FVector readNextFrame();
	
	//new read last reached frame from prev anim
	FVector readPrevAnimationReachedFrame();

	void overrideNextFrameA(FVector &framePos);
	bool animationCycleWasComplete();

	void processProjectOffset(FVector &offsetMade);
	FVector getProjectionHipOffsetTimed();

	
	void tryPushFront(FVector &currentLocationRelative);
	
	//neu
	void projectNextFrameIfNeeded(UWorld *world, MMatrix &actorMatrix);
	void projectNextFrameIfNeeded(
		UWorld *world,
		MMatrix &actormatrix,
		float velocity,
		FVector &lookdir
	);

private:
	//neu
	FVector projectionHipOffset;
	FVector bTarget;
	bool bIsSetToAutoOverride = false;

	//vorher
	bool reachedTime(float timeCheck);

	class KeyFrameAnimation framesA;
	
	class TargetInterpolator interpolateB;

	bool isAnimationAPlaying = true;
	float deltaTime = 0.0f;

	bool cycleComplete = false;
	bool blocked = false;

	

	
	KeyFrameAnimation &prevAnimation();

	
	float reachTime();

	FVector aReachedTickFrame;

	float sinusFlyingOffset(float time, float width);

	bool isRunningAnimation = false;

	FVector flyingOffset();

public:
	void setRunning(bool b);
};
