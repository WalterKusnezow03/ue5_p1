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
	

	FVector interpolate(float DeltaTime);

	//From KeyFrame Class
	FVector readNextFrame();
	
	//new read last reached frame from prev anim
	FVector readPrevAnimationReachedFrame();

	bool nextFrameMustBeGrounded();
	void overrideNextFrameA(FVector &framePos);

	bool currentAndNextForBOverriden();
	void tryOverrideCurrentAndNextFrameAnimB(FVector &currentNew, FVector &nextNew, float timeToFrameWanted);

	bool nextFrameIsProjected();
	bool animationCycleWasComplete();

	void processProjectOffset(FVector &offsetMade);
	FVector getProjectionHipOffsetTimed();

	//neu
	void tryPushFront(FVector &currentLocationRelative);

private:
	//neu
	FVector projectionHipOffset;

	//vorher
	bool reachedTime(float timeCheck);

	class KeyFrameAnimation framesA;
	
	class TargetInterpolator interpolateB;

	bool isAnimationAPlaying = true;
	float deltaTime = 0.0f;

	bool cycleComplete = false;

	bool currentAndNextOverridenB = false;

	
	KeyFrameAnimation &prevAnimation();

	FVector interpolateAtarget(float DeltaTime);
	FVector interpolateBtarget(float DeltaTime);
	float reachTime();

	FVector aReachedTickFrame;

	/**
	 * 
	 * die idee:
	 * eine kette die abgespielt wird und
	 * die andere immer ein target ggf hat
	 * das reicht erstmal so.
	 * 
	 * man muss ja insgesamt nur abbilden das getauscht wird. 
	 * 
	 * es ist noch unklar ob das der richtige ansatz ist
	 * wenn der switch stattfindet muss noch der richtige frame auch noch
	 * gesetzt wird. Heisst, bei flip muss der frame UNBEDINGT
	 * Overriden werden vom actor, sehr wichtig!
	 * Und vielleicht wäre es wirklich besser das auch dieser klasse
	 * hier zu überlassen
	 * und nicht im actor ggf zu überlassen? oder?
	 * also wenn frames B dann muss auch das target UND Start overriden werden
	 * 
	 */
	
};
