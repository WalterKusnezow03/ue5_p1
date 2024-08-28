// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API bezierCurve
{
public:
	bezierCurve();
	~bezierCurve();

	void calculatecurve(std::vector<FVector2D> &ref, std::vector<FVector2D> &output);

private:
	constexpr static const float STEP_SIZE = 0.01f;

	int predictFinalCurveElementCount(std::vector<FVector2D> &anchors);
	void createContinuityCurve(std::vector<FVector2D> &anchors);

	void processAllPoints(std::vector<FVector2D> &points, std::vector<FVector2D> &output);
	void process4Points(std::vector<FVector2D> &points, int offset, std::vector<FVector2D> &output);
	void fillGaps(std::vector<FVector2D> &vec);

	void linearInterpolate(FVector2D &a, FVector2D &b, std::vector<FVector2D> &container);

	FVector2D FVector2DFourAnchorBezier(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c,
		FVector2D &d,
		float skalar
	);



};
