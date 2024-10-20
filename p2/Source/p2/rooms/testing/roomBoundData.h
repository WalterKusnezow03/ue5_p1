// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API roomBoundData
{
public:
	roomBoundData(int xin, int yin, int xscalein, int yscalein, int numberIn);
	int number = 0;

	~roomBoundData();

	void connect(roomBoundData &other);
	void addDoorPosition(int xIn, int yIn);
	void addWindowPosition(int xIn, int yIn);

	std::vector<FVector> relativeWindowPositionsCm();
	std::vector<FVector> relativeDoorPositionsCm();

	int xpos();
	int ypos();
	int xScale();
	int yScale();
	int xMax();
	int yMax();


private:
	int x = 0;
	int y = 0;
	int xscale = 0;
	int yscale = 0;

	std::vector<FVector> doorPositions;
	std::vector<FVector> windowPositions;

	bool contains(std::vector<FVector> &other, int xin, int yin);
	bool contains(std::vector<FVector> &other, FVector pos);

	void clamp(FVector &other);

	void defineOverlap(
    	roomBoundData &other,
		int otherX,
		int otherY,
		int otherXmax,
		int otherYmax,
		int thisX,
		int thisY,
		int thisXmax,
		int thisYmax,
		bool xyflipped
	);
	bool oneApart(int a, int b, bool &flip);
	// std::vector<roomBoundData *> &order);

	std::vector<roomBoundData *> connectedNeighbors;
	bool hasNeighbor(roomBoundData *other);
};
