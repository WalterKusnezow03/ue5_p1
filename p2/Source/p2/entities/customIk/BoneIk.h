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

	void tickAndBuild(UWorld *world, FVector &offset, float etha, float displayTime);

private:
	MMatrix hip;
	MMatrix knee;
	MMatrix foot;
	FVector toFootTip;

	void getData(std::vector<MMatrix*> &dataout, FVector &outVector);
	void setEtha(float etha);
};
