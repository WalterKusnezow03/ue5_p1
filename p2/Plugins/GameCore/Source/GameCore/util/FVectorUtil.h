// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GAMECORE_API FVectorUtil
{
public:
	//very important!
	static FRotator lookAt(FVector ownlocation, FVector TargetLocation);
	static FRotator lookRotation(FVector &other);

	static FVector randomOffset(int range);
	static FVector calculateNormal(FVector &a, FVector &b, FVector &c);
	static FVector calculateCenter(FVector &a, FVector &b, FVector &c);
	static FVector calculateCenter(FVector &a, FVector &b, FVector &c, FVector &d);
	static FVector calculateCenter(std::vector<FVector> &vec);
	static FVector calculateCenter(const TArray<FVector> &vec);
	static FVector randomScale(int lowerRange, int higherRange, bool symetric);
	static FVector abs(FVector &input);
	static FRotator randomRotation(FVector ownLocation);

	static FVector2D randomOffset2D(int range);
	static FVector2D randomOffset2D(int lower, int higher);

	static bool edgeIsVertical(FVector &A, FVector &B);
	static bool directionIsVertical(FVector &A);

	static int randomNumber(int range);
	static int randomNumberAbs(int range);
	static int randomNumber(int lower, int higher);
	static float randomFloatNumber(int lower, int higher);
	static void fillWithRandomNumbers(std::vector<int> &nums, int count, int lower, int higher);

	static float Dist(FVector2D &a, FVector2D &b);
	static int quadraticDist(FVector &a, FVector &b);

	/// @brief returns std::infinity
	/// @return infinity
	static float infinity();
	static int intInfinity();

	static float AreaTriangle(const FVector &v0, const FVector &v1, const FVector &v2);


	static void ScaleDownByWidth(
        const TArray<FVector> &circle,
        TArray<FVector> &OutInnerCircle,
        float width
    );
    static void ScaleDownByWidth(
        const TArray<FVector> &circle,
        TArray<FVector> &OutInnerCircle,
        float width,
        const FVector &center
    );

private:
	FVectorUtil();
	~FVectorUtil();
};
