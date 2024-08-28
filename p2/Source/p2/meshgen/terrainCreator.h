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
	class UWorld *worldPointer;
	

	class chunk{
		public:
			chunk(int xPos, int yPos);
			~chunk();

			void applyHeight(
				int xValue,
				int ylowerIndex,
				int ytopIndex,
				int yBottomValue,
				int yTopValue
			);

		private:
			std::vector<std::vector<FVector>> innerMap;
			int x;
			int y;
	};

	std::vector<std::vector<terrainCreator::chunk>> map;

	std::vector<FVector2D> createSamplePoints();
	void debugDrawCurve(UWorld *world, std::vector<FVector2D> &vec, FColor color);

	
	

	void applyCurveToMap(std::vector<FVector2D> &vec);
	int validateX(int a);
	int validateY(int a);
	bool isXTouple(FVector2D &a, FVector2D &b);
	void processTouple(FVector2D &a, FVector2D &b);
};
