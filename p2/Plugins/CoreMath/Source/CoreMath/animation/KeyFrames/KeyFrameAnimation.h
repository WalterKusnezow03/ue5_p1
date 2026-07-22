// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/TransformInterpolator.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "KeyFrame.h"

class UKeyFrameAnimationAsset;
struct FKeyFrameAsset;
struct FKeyFrameAssetArray;

/**
 * will store keyframes for a bone movement for example
 */
class COREMATH_API KeyFrameAnimation
{
protected:
	//initialized as first member
	std::vector<KeyFrame> frames; //might be replaced with frame class with time stamp


public:
	KeyFrameAnimation();
	KeyFrameAnimation(bool loopIn);
	virtual ~KeyFrameAnimation();

	KeyFrameAnimation(UKeyFrameAnimationAsset *asset);
	void ConstructFrom(UKeyFrameAnimationAsset *asset);

	//optional to change
	void SetLoopFlag(bool loopIn);
	bool LoopFlagged();

	void addFrame(FVector position, float timeFromLastFrame);
	void addFrame(FVector position, float timeFromLastFrame, bool mustBeGrounded);
	void addFrame(
		FVector position,
		float timeFromLastFrame,
		bool mustBeGrounded,
		float clampDistance
	);
	void addFrame(const FKeyFrameAsset &frame);
	void addAll(const TArray<FKeyFrameAsset> *ptr);
	void addAll(const TArray<FKeyFrameAsset> &array);
	void addAll(const FKeyFrameAssetArray &arrayIn);
	void clear();

	FVector interpolate(float DeltaTime);
	FVector interpolate(float DeltaTime, FVector currentPos);
	FVector interpolateWorld(
		float DeltaTime,
		FVector currentPosWorld,
		MMatrix &actor
	);

	bool nextFrameMustBeGrounded();
	FVector readNextFrame();
	
	
	float totalLength() const;
	bool nextFrameIsProjected();

	/// @brief never true, if animation marked looping
	virtual bool reachedLastFrameOfAnimation();
	void overrideCurrentStartingFrame(FVector &somePoisition);
	void overrideNextFrame(FVector &framePos);
	
	void restart();

	void skipAnimationOnce(FVector start, FVector end);
	void skipAnimationOnceWorld(MMatrix &actor, FVector start, FVector end);







	/**
	 * --- START new section for rotation on frames! ---
	 */
	void rotateNextFrames(float singedAngleYaw);
	void resetRotationOnFramesFlag();

	void resetAnimationToStartAndResetRotation();

 
	void useHermiteSplineInterpolation(bool flag);

protected:

	virtual void OnFinish() {
		//to be overriden.
	};

private:
	void addRotationToFrame(FVector &localFrameToRotate);
	bool rotateFramesBasedOnAngle = false;
	MMatrix rotateFramesMatrix;
	/**
	 * --- END new section for rotation on frames! ---
	 */


	bool DEBUGDRAW_RAYCAST = false;

	float raycastVerticalStartOffsetAdd = 250.0f; //400.0f
	float raycastScaleVector = 5000.0f;

	FVector latestInterpolation;
	bool loop = true;
	bool reachedEndFrameFlag = false;

	float totalLengthSave = 0.0f;
	

	bool frameIsProjected = false;

	

	
	//float deltaTime = 0.0f;
	//float lastDeltatime = 0.0f;

	int frameIndex = 0;
	int nextFrameIndex = 1;

	void updateFrameIndex();
	bool canAnimate();
	bool hasAnyFrames()const;




	class TransformInterpolator interpolator;
	// class TransformInterpolator interpolator;
	void updateFrameInterpolator();



	bool performRaycast(UWorld *world, FVector &Start, FVector &dir, FVector &outputHit);

	

	void updateAverageVelocity();
	float averageVelocityOfFrames = 0.0f;
	float linearVelocity(FVector &a, FVector &b, float timeBetween);

public:
	float averageVelocity();
	void scaleToVelocityInCms(float VcmPerSecond);

	//between n - 2 and n - 1, not n, if animation marked looping.
	//else: n and n - 1 (starting and ending frame checked same internally)
	FVector directionOfLastLogicalTwoFrames();
protected:
	bool IndexValid(int i);
	FVector DirectionBetweenFrames(int i, int j);
	bool FramesAreSame(int i, int j);

	// ---- TO STRING DEBUG ----
public:
	FString ToString();

	FString StatusInfo();

	// ---- DEBUG ----
	void GetAllKeyFrames(TArray<FVector> &array);

public:
	// --- SCALING ---

	//scales the animation with a given scalar
	//and its current progress
	void ScaleTimeWithScalar(float scalar);



	
};
