// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/animation/DoubleKeyFrameAnimation.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"
#include "p2/entities/customIk/bonePackage/BoneControllerStates.h"
#include "p2/entities/customIk/bonePackage/ArmMotionStates.h"
#include "p2/entities/customIk/bonePackage/TwoBone.h"
#include "p2/entities/customIk/MMatrix.h"

/**
 * This class will manage Bones of itself
 * and all amtricies creating one skelleton
 * 
 * just as the bones, the initial look dir is towards the positive x axis!
 * 
 */
class P2_API BoneController
{
public:
	BoneController &operator=(BoneController &other);

	BoneController();
	BoneController(float boneLengthCm, float armScaleInCm);
	BoneController(BoneController &other);

	~BoneController();

	//must be ticked from outside
	void Tick(float DeltaTime, UWorld *world, FVector overrideLocation);
	void Tick(float DeltaTime, UWorld *world);
	void SetLocation(FVector &vector);
	FVector GetLocation();
	void LookAt(FVector TargetLocation);
	void yawRotate(float degree);

	MMatrix currentTransform();

	void SetControllerState(BoneControllerStates state);

	void attachCarriedItem(AcarriedItem *carriedItem);

private:
	class AcarriedItem *attachedCarriedItem;

	bool isRunning = false; //debug
	float velocity = 200.0f;

	FColor leg1Color = FColor::Red;
	FColor leg2Color = FColor::Blue;
	FColor arm1Color = FColor::Purple;
	FColor arm2Color = FColor::Emerald;

	//limb indices, dont change
	int FOOT_1 = 1;
	int FOOT_2 = 2;
	int SHOULDER_1 = 3;
	int SHOULDER_2 = 4;



	//motion state
	BoneControllerStates currentMotionState;
	
	//hand motion state
	ArmMotionStates currentArmMotionState;



	bool leg1isPlaying;

	

	float addVelocityBasedOnState();

	MMatrix ownLocation;
	MMatrix ownOrientation;

	//foot
	MMatrix ownLocationFoot1; //real world location end foot 1
	MMatrix ownLocationFoot2; //real world location end foot 2
	MMatrix hip1MatrixOffset; //local offset to hip com
	MMatrix hip2MatrixOffset; //local offset to hip com

	//hands
	MMatrix shoulder1MatrixOffset;
	MMatrix shoulder2MatrixOffset;
	MMatrix ownLocationHand1; //real world location end hand 1
	MMatrix ownLocationHand2; //real world location end hand 2

	//socket for item
	MMatrix weaponMatrixOffset;



	//leg locomotion keys
	class DoubleKeyFrameAnimation legDoubleKeys_1;
	class DoubleKeyFrameAnimation legDoubleKeys_2;

	//arm climb locomotion keys
	class DoubleKeyFrameAnimation armClimbKeys_1;

	//arm animation keys move to target 
	class KeyFrameAnimation armTransitKeys_1;
	class KeyFrameAnimation armTransitKeys_2;



	float legScaleCM = 150.0f;
	float armScaleCM = 100.0f;

	class TwoBone leg1;
	class TwoBone leg2;

	class TwoBone arm1;
	class TwoBone arm2;




	void setupBones();
	void setupAnimation();

	void TickBuildNone(float DeltatTime);
	void TickLocomotion(float DeltaTime);
	
	void TickLocomotionClimb(float DeltaTime);
	void TickLocomotionClimbAdjust(float DeltaTime);

	
	void TickArms(float DeltaTime);

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
	MMatrix offsetMatrixByLimb(int limb);
	MMatrix offsetInverseMatrixByLimb(int limb);

	MMatrix *findEndEffector(int limbIndex);

	TwoBone *findBone(int limbIndex);
	FColor limbColor(int limbindex);

	

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




	//new section for climbing
	void TickLocomotionClimbAll(float DeltaTime);
	void playForwardAndBackwardKinematicAnimSynchronized(
		float DeltaTime,
		std::vector<int> limbs,
		std::vector<DoubleKeyFrameAnimation *> animations
	);

	

	bool climb_hand1cycleComplete = false;
	bool climb_setHandTarget = false;
};
