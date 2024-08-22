// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API layoutCreator
{
public:
	layoutCreator();
	~layoutCreator();

	std::vector<std::vector<int>> generateLayout(int sizeX, int sizeY);

private:
	class roomBounds{
		public:
			
			roomBounds(int xIn, int yIn);
			~roomBounds();
			int xValue();
			int yValue();

		private:
			int x;
			int y;
	};

	void addRoom(std::vector<std::vector<int>> &map, int &nextX, int &nextY, int roomNum);
	bool isFree(int a);
	void blockArea(std::vector<std::vector<int>> &map, int x, int y, int xSize, int ySize, int roomNum);

	roomBounds pickRoom(int xMax, int yMax);



};
