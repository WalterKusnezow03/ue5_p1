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

	static const bool PLOTTING_ENABLED = false; // false;
	static const int CHUNKSIZE = 10;
	static const int ONEMETER = 100;
	int chunkNum();
	void createterrain(UWorld *world, int meters);

	//apply terrain
	void applyTerrainDataToMeshActors(std::vector<AcustomMeshActor *> &actors);

private:
	class chunk{
		public:
			chunk(int xPos, int yPos);
			~chunk();

			FVector position();

			void applyHeightBeetwennVerticalPositions(FVector2D a, FVector2D b);

			std::vector<FVector2D> getXColumAnchors(int xColumn);
			std::vector<FVector2D> getYRowAnchors(int yRow);

			void plot(UWorld *world);
			void plotCorners(UWorld * world);

			void applyIndivualVertexIndexBased(
				int xIn,
				int yIn,
				float newHeight,
				bool override
			);

			std::vector<std::vector<FVector>> &readMap();

			std::vector<std::vector<FVector>> readAndMerge(
				chunk *top,
				chunk *right,
				chunk *topRight
			);
			bool xIsValid(int a);
			bool yIsValid(int a);

		private:
			std::vector<std::vector<FVector>> innerMap;
			int x;
			int y;

			int clampInnerIndex(int a);
			
			
			int jumpHeight(); // add up to terrain heigh when needed
			int heightAdd();
			bool jumpOfInterest(FVector &a, FVector &b);

			int convertToInnerIndex(int value);
			int clampOuterYIndex(FVector2D &a);
			bool isInBounds(FVector &a);

			int xPositionInCm();
			int yPositionInCm();

			std::vector<FVector> readFirstXColumn();
			std::vector<FVector> readFirstYRow();
			FVector readBottomLeftCorner();
	};

	class UWorld *worldPointer;

	std::vector<std::vector<terrainCreator::chunk>> map;

	std::vector<FVector2D> createSamplePoints();
	void debugDrawCurve(UWorld *world, std::vector<FVector2D> &vec, FColor color);

	void plotAllChunks(UWorld *world);

	void processTopViewBezierCurve(std::vector<FVector2D> &bezier);
	void applyTopViewCurveToMap(std::vector<FVector2D> &vec);
	int validateIndex(int a);
	bool isXTouple(FVector2D &a, FVector2D &b);
	void processTouple(FVector2D &a, FVector2D &b);

	void smooth3dMap();
	void cleanValues(std::vector<FVector2D> &vec);
	void fillGaps(std::vector<FVector2D> &vec);

	void applyXColumnToMap(int index, std::vector<FVector2D> &column);
	void applyYRowToMap(int index, std::vector<FVector2D> &row);

	bool veriyIndex(int a);
	int cmToChunkIndex(int a);

	int cmToMeter(int a);
	int meterToInnerChunkIndex(int a);
	int cmToInnerChunkIndex(int a);

	void upScalePoints(std::vector<FVector2D> &points, float factor);
	void offsetPoints(std::vector<FVector2D> &vec, FVector2D offset);


};
