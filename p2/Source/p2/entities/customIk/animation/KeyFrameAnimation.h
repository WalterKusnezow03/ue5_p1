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
	~KeyFrameAnimation();

	void addFrame(FVector position, float timeFromLastFrame);
	void addFrame(FVector position, float timeFromLastFrame, bool mustBeGrounded);
	void addFrame(
		FVector position,
		float timeFromLastFrame,
		bool mustBeGrounded,
		float clampDistance
	);
	FVector interpolateOld(float DeltaTime);
	FVector interpolate(float DeltaTime);

	float lastDeltaTime();

	bool nextFrameMustBeGrounded();
	FVector readNextFrame();
	FVector readPrevFrame();

	void overrideNextFrame(FVector &framePos);

	float totalLength();

	bool nextFrameIsProjected();

private:
	float totalLengthSave = 0.0f;
	FVector targetCopy;

	bool frameIsProjected = false;

	std::vector<KeyFrame> frames; //might be replaced with frame class with time stamp
	float deltaTime = 0.0f;
	float lastDeltatime = 0.0f;

	int frameIndex = 0;

	

	void clampIndex();
	int nextFrameIndex();
	int prevFrameIndex();

	bool canAnimate();
	bool hasAnyFrames();

	float skalar(float timeDistance);



	class TargetInterpolator interpolator;
	void updateFrameInterpolator();
};
