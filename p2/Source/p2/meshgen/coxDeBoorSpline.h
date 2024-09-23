// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API coxDeBoorSpline
{
public:
	coxDeBoorSpline();
	~coxDeBoorSpline();

	void calculatecurve(
		std::vector<FVector2D> &controls,
		TVector<FVector2D> &output,
		int totalLength,
		int gapBetweenInterpolatedPoints
	);
private:
	float CoxDeBoor(int i, int p, float t, const std::vector<float> &knotVector);

	FVector2D interpolateBSpline(
		float t,
		std::vector<FVector2D> &controlPoints,
		int degree,
		const std::vector<float> &knotVector
	);

	void createOutput(
		std::vector<FVector2D> &controlPoints,
		int degree,
		const std::vector<float> &knotVector,
		TVector<FVector2D> &output,
		int distanceTotal,
		int gapBetween
	);
	std::vector<float> generateClampedKnotVector(int n, int p);
};
