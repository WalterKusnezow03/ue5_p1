// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API FVectorUtil
{
public:
	static FVector randomOffset(int range);
	static FRotator lookAt(FVector ownlocation, FVector TargetLocation);
	static int randomNumber(int range);
	static int randomNumber(int lower, int higher);

	static FRotator randomRotation(FVector ownLocation);

	static FVector randomScale(int lowerRange, int higherRange, bool symetric);

private:
	FVectorUtil();
	~FVectorUtil();
};
