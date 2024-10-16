// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "p2/rooms/layoutCreator/roomBounds.h"
#include "CoreMinimal.h"

/**
 * 
 */
class P2_API layoutMaker
{
public:
	layoutMaker();
	~layoutMaker();

	void makeLayout(
		int xMeters,
		int yMeters,
		std::vector<TTouple<int, int>> &possibleSizes, 
		std::vector<roomBounds> &output
	);

	

private:
	std::vector<std::vector<roomBounds*>> pointerField;
	std::vector<roomBounds *> owningRoomsVec;
	
	void clearAndFill(int x, int y);
	void clearField();

	bool fieldIsFilled();

	void tryFit(int xSize, int ySize);
	bool canFit(int fromX, int fromY, int xSize, int ySize);
	void fill(int fromX, int fromY, int xSize, int ySize, roomBounds *room);

	void createDoorsAndWindows();
	void makeDoors();

	void makeWindows();
	bool isOuterEdge(int i);

	void checkFree(int xf, int yf, int xto, int yto, std::vector<FVector> &output);
	int validIndex(int i);
};
