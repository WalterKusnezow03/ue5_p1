// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API terrainCreator
{
public:
	terrainCreator();
	~terrainCreator();


	static const int CHUNKSIZE = 10;
	void createterrain(UWorld *world, int meters);

private:
	class chunk{
		public:
			chunk();
			~chunk();
		
		private:
			std::vector<std::vector<FVector>> innerMap;
	};

	std::vector<std::vector<terrainCreator::chunk>> map;

	std::vector<FVector2D> createSamplePoints();

	int predictFinalCurveElementCount(std::vector<FVector2D> &anchors);

	void createContinuityCurve(std::vector<FVector2D> &anchors);

	void processAllPoints(std::vector<FVector2D> &points, std::vector<FVector2D> &output);
	void process4Points(std::vector<FVector2D> &points, int offset, std::vector<FVector2D> &output);

	float yOffsetFourAnchorBezier(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c,
		FVector2D &d,
		float skalar
	);

	FVector2D FVector2DFourAnchorBezier(
		FVector2D &a,
		FVector2D &b,
		FVector2D &c,
		FVector2D &d,
		float skalar
	);

	void debugDrawCurve(UWorld *world, std::vector<FVector2D> &vec, FColor color);

	class UWorld *worldPointer;
	void fillGaps(std::vector<FVector2D> &vec);
	void linearInterpolate(FVector2D &a, FVector2D &b, std::vector<FVector2D> &container);
};
