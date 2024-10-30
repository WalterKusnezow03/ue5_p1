// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * polygon dedicated for the vornoi algorythm, will be always convex
 */
class P2_API VPolygon
{
public:
	VPolygon();
	VPolygon(std::vector<FVector> &verteciesIn);

	~VPolygon();


private:
	std::vector<FVector> vertecies;

	void sort();
	void ComputeConvexHull(std::vector<FVector> &points);
	bool IsClockwise(const FVector &a, const FVector &b, const FVector &c);
};
