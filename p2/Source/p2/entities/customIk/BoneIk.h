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
	void build(UWorld *world, FVector &offset, FColor color, float displayTime);

	void tickAndBuild(UWorld *world, FVector &offset, float displayTime); //only move
	void tickAndBuild(UWorld *world, FVector &offset, float etha, float displayTime); //move and etha
	

	//move, etha and leg pitch
	void tickAndBuild(
		UWorld *world,
		FVector &offset, // offset data in world
		float etha,
		float legPitchThete, 
		float displayTime,
		FColor color
	);

	void tickMotion(UWorld *world, float deltaTime, FVector &offset, FColor color);
	float halfTimePhase();

	bool halfIsReached();

	FVector movedLastTick();

	//rotation of whole bone - starting node
	void roll(float degree);
	void pitch(float degree);
	void yaw(float degree);

	void setEtha(float etha);

private:
	MMatrix hip;
	MMatrix knee;
	MMatrix foot;
	FVector toFootTip;

	void getData(std::vector<MMatrix*> &dataout, FVector &outVector);
	
	void setEtha(float etha, float legPitchThetaRadian); //new testing

	bool halfReached = false;

	//testing needed
	float degreePerSecond = 200;
	float deg = 0.0f;
	float legSwingRadian = 0.0f;



	//testing
	FVector prevFootPos;
	FVector movedDir;
};
