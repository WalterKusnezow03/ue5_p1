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

private:
	FVectorUtil();
	~FVectorUtil();
};
