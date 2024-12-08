// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetInterpolator.h"
#include "KeyFrame.h"

/**
 * will store keyframes for a bone movement for example
 */
class P2_API KeyFrameAnimation
{
public:
	KeyFrameAnimation();
	KeyFrameAnimation(bool loopIn);
	~KeyFrameAnimation();

	void addFrame(FVector position, float timeFromLastFrame);
	void addFrame(FVector position, float timeFromLastFrame, bool mustBeGrounded);
	void addFrame(
		FVector position,
		float timeFromLastFrame,
		bool mustBeGrounded,
		float clampDistance
	);
	
	FVector interpolate(float DeltaTime);


	bool nextFrameMustBeGrounded();
	FVector readNextFrame();
	
	
	float totalLength();
	bool nextFrameIsProjected();
	bool reachedLastFrameOfAnimation();
	void tryPushFront(FVector &somePoisition, float time);
	void overrideNextFrame(FVector &framePos);
	void restart();

private:
	FVector latestInterpolation;
	bool loop = true;
	bool restarted = true;
	bool reachedEndFrameFlag = false;

	float totalLengthSave = 0.0f;
	

	bool frameIsProjected = false;
	

	std::vector<KeyFrame> frames; //might be replaced with frame class with time stamp
	float deltaTime = 0.0f;
	float lastDeltatime = 0.0f;

	int frameIndex = 0;
	int nextFrameIndex = 1;

	void updateFrameIndex();
	bool canAnimate();
	bool hasAnyFrames();

	



	class TargetInterpolator interpolator;
	void updateFrameInterpolator();





//neu projektion auch hier
public:
	bool projectNextFrameToGroundIfNeeded(UWorld *world, MMatrix &actorMatrix, FVector &offsetMade);
private:
	void projectToGround(UWorld *world, MMatrix &actorTransform, FVector &frameToProject, FVector &offsetMade);
	bool performRaycast(UWorld *world, FVector &Start, FVector &dir, FVector &outputHit);
};
