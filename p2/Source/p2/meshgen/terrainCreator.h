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
	static const int ONEMETER = 100;
	void createterrain(UWorld *world, int meters);

private:
	class UWorld *worldPointer;
	

	class chunk{
		public:
			chunk(int xPos, int yPos);
			~chunk();

			void applyHeight(FVector2D a, FVector2D b);

			std::vector<FVector2D> getXColumAnchors(int xColumn);
			std::vector<FVector2D> getYRowAnchors(int yRow);

			void plot(UWorld *world, FColor color);
			void plot(UWorld *world, FColor color, int zOffset);

		private:
			std::vector<std::vector<FVector>> innerMap;
			int x;
			int y;

			int clampInnerIndex(int a);
			
			bool xIsValid(int a);
			bool yIsValid(int a);
			int jumpHeight(); // add up to terrain heigh when needed
			bool jumpOfInterest(FVector &a, FVector &b);



			int convertToInnerIndex(int value);
			int clampOuterYIndex(FVector2D &a);
	};

	std::vector<std::vector<terrainCreator::chunk>> map;

	std::vector<FVector2D> createSamplePoints();
	void debugDrawCurve(UWorld *world, std::vector<FVector2D> &vec, FColor color);

	void plotAllChunks(UWorld *world, FColor color);
	

	void applyTopViewCurveToMap(std::vector<FVector2D> &vec);
	int validateIndex(int a);
	bool isXTouple(FVector2D &a, FVector2D &b);
	void processTouple(FVector2D &a, FVector2D &b);

	void smooth3dMap();
	void cleanValues(std::vector<FVector2D> &vec);

	void applyXColumnToMap(int index, std::vector<FVector> &column);
	void applyYRowToMap(int index, std::vector<FVector> &row);
};
