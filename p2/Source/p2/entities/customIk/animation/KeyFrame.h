// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API KeyFrame
{
public:
	KeyFrame(FVector positionIn, float timeFromLastFrameIn);
	KeyFrame(FVector positionIn, float timeFromLastFrameIn, bool groundedIn);
	~KeyFrame();

	FVector readposition();
	float timeToFrame();
	bool mustBeGrounded();

	void setTime(float timeFromLastFrameIn);

private:
	FVector position;
	float timeFromLastFrame;
	bool grounded = false;
};
