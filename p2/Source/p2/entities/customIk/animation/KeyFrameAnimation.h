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
	FVector readPrevFrame();
	FVector readLastFrameOfAnimation();

	void overrideNextFrame(FVector &framePos);
	void overrideCurrentAndNextFrame(FVector &current, FVector &target);

	void resetIndex();

	float totalLength();

	bool nextFrameIsProjected();

	bool reachedLastFrameOfAnimation();

	void tryPushFront(FVector &somePoisition);

private:
	bool loop = true;
	bool reachedEndFrameFlag = false;

	float totalLengthSave = 0.0f;
	FVector targetCopy;

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
};
