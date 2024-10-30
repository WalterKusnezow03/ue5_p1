// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMatrix.h"

/**
 * will hold 3 matricies to crate an arm or an leg
 */
class P2_API BoneIk
{
public:
	BoneIk();
	~BoneIk();

	void setupBones(float completeDistance);
	void build(UWorld *world, FVector &offset, FColor color, float displayTime); //in use
	void build(UWorld *world, MMatrix &offsetAndRotation, FColor color, float displayTime); //must be tested

	//move, etha and leg pitch
	void tickAndBuild(
		UWorld *world,
		MMatrix &offsetMatrix, // offset data in world
		float etha,
		float legPitchThetaRadian,
		float displayTime,
		FColor color
	);

	void tickLegMotion(UWorld *world, float deltaTime, MMatrix &offsetMatrix, FColor color); 
	float halfTimePhase();

	bool halfIsReached();

	FVector movedLastTick();

	//rotation of whole bone - starting node
	void rotateFirstLimbDeg(float xDeg, float yDeg, float zDeg);
	void rotateFirstLimbRad(float xDeg, float yDeg, float zDeg);

	void rotateLastLimbDeg(float xDeg, float yDeg, float zDeg);
	void rotateLastLimbRad(float xDeg, float yDeg, float zDeg);


	// etha functions and moving towards targets
	void setEtha(float etha);

	
	void rotateEndToTarget(FVector &vec, FVector &weight);

private:
	float angleFromEtha(float etha);
	float createHipAngle(float angle);
	float createKneeAngle(float angle);

	//calculating angles
	float yawAngleTo(FVector &localTarget);
	float pitchAngleTo(FVector &localTarget);
	float rollAngleTo(FVector &localTarget);

	void resetAllRotations();

	int flipRotation(float aX, float aY, float oX, float oY);

	/// @brief hip or shoulder
	MMatrix hip;
	
	/// @brief knee or ellbow
	MMatrix knee;
	
	/// @brief foot or hand
	MMatrix foot;

	///@brief foot tip or finger tips end
	FVector toFootTip;

	///@brief will save the total length of the bone for later usuage, dont change, only constructor
	float totalBoneLengthCopy = 0.0f;

	void getMatricies(std::vector<MMatrix*> &dataout, FVector &outVector);
	
	void setEtha(float etha, float legPitchThetaRadian);

	bool halfReached = false;

	//testing needed
	float degreePerSecond = 200;
	float deg = 0.0f;
	float legSwingRadian = 0.0f;



	//testing
	FVector prevFootPos;
	FVector movedDir;
};
