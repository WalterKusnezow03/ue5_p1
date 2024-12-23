// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/animation/DoubleKeyFrameAnimation.h"
#include "p2/entities/customIk/bonePackage/BoneControllerStates.h"
#include "p2/entities/customIk/bonePackage/TwoBone.h"
#include "p2/entities/customIk/MMatrix.h"

/**
 * This class will manage Bones of itself
 * 
 * it will hold Bones (in a list or maybe just two?)
 * bones and could create them on Construct	
 * 
 * the ownLocation and ownOrientation
 * must be indepandant of anything else, 
 * 
 */
class P2_API BoneController
{
public:
	BoneController &operator=(BoneController &other);

	BoneController();
	BoneController(float boneLengthCm);
	BoneController(BoneController &other);

	~BoneController();

	//must be ticked from outside
	void Tick(float DeltaTime, UWorld *world, FVector overrideLocation);
	void Tick(float DeltaTime, UWorld *world);
	void SetLocation(FVector &vector);
	void LookAt(FVector TargetLocation);

	MMatrix currentTransform();

	void SetControllerState(BoneControllerStates state);

private:
	FColor leg1Color = FColor::Red;
	FColor leg2Color = FColor::Blue;
	FColor arm1Color = FColor::Purple;
	FColor arm2Color = FColor::Emerald;



	BoneControllerStates currentControllerState;

	bool leg1isPlaying;

	bool isRunning = false; //debug
	float velocity = 200.0f;

	float addVelocityBasedOnState();

	MMatrix ownLocation;
	MMatrix ownOrientation;

	MMatrix ownLocationFoot1; //real world location end foot 1
	MMatrix ownLocationFoot2; //real world location end foot 2

	MMatrix hip1MatrixOffset; //local offset to hip com
	MMatrix hip2MatrixOffset; //local offset to hip com


	//testing hands.
	MMatrix shoulder1MatrixOffset;
	MMatrix shoulder2MatrixOffset;
	MMatrix ownLocationHand1; //real world location end hand 1
	MMatrix ownLocationHand2; //real world location end hand 2





	class DoubleKeyFrameAnimation legDoubleKeys_1;
	class DoubleKeyFrameAnimation legDoubleKeys_2;

	class DoubleKeyFrameAnimation armClimbKeys_1;

	float legScaleCM = 150.0f;
	float armScaleCM = 100.0f;

	class TwoBone leg1;
	class TwoBone leg2;

	class TwoBone arm1;
	class TwoBone arm2;

	//very experimental




	void setupBones();
	void setupAnimation();

	void TickBuildNone(float DeltatTime);
	void TickLocomotion(float DeltaTime);
	void TickMoveToTarget(float DeltaTime);

	void TickLocomotionClimb(float DeltaTime);
	void TickLocomotionClimbAdjust(float DeltaTime);

	void TickArmsNone(float DeltaTime);
	void TickLegsNone(float DeltaTime);

	void TickLimbNone(int limbIndex, float DeltaTime);

	void updateHipLocation(MMatrix &updatetHipJointMat, int leg);
	void transformFromWorldToLocalCoordinates(FVector &position, int leg);


	//geföhrlich
	class UWorld *world = nullptr;
	UWorld* GetWorld();


	void buildRawAndKeepEndInPlace(
		TwoBone &boneIk,
		MMatrix &legTransform,
		float deltaTime,
		FColor color,
		int leg
	);

	

	

	MMatrix currentTransform(int leg);
	MMatrix currentTransform(MMatrix &offset);
	MMatrix currentFootTransform(MMatrix &foottranslationToRotate);



	TwoBone *findBone(int limbIndex);
	FColor limbColor(int limbindex);

	int FOOT_1 = 1;
	int FOOT_2 = 2;
	int SHOULDER_1 = 3;
	int SHOULDER_2 = 4;

	void playForwardAndBackwardKinematicAnim(
		TwoBone &bone,
		DoubleKeyFrameAnimation &frames,
		MMatrix &footMatrix, // MMatrix foot transform
		float DeltaTime,
		FColor color,
		int leg // bein 1 oder 2 fürs erste, könnte auch liste werden
	);

	void playForwardKinematicAnim(
		TwoBone &bone,
		DoubleKeyFrameAnimation &frames,
		MMatrix &footMatrix,
		float DeltaTime,
		FColor color,
		int leg
	);

	void playBackwardKinematicAnim(
		TwoBone &bone,
		DoubleKeyFrameAnimation &frames,
		MMatrix &footMatrix, // MMatrix foot transform
		float DeltaTime,
		FColor color,
		int leg
	);

	//single forward ik action
	
	void playForwardKinematicAnim(
		TwoBone &bone, 
		KeyFrameAnimation &frames, 
		float DeltaTime,
		int limbIndex
	);






};
