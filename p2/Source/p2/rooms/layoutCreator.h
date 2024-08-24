// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/rooms/room.h"
#include <list>

class Aroom;

/**
 * 
 */
class P2_API layoutCreator
{
public:
	layoutCreator();
	~layoutCreator();

	void createRooms(UWorld *worldIn, FVector location);

private:
	

	class roomBounds{
		public:
			void updateData(int xPos, int yPos, int xSize, int ySize);

			//create room type too!, and door positions
			int number;
			roomBounds(int xIn, int yIn, int num);
			~roomBounds();
			int xpos();
			int ypos();

			int xscale();
			int yscale();

			void addNeighbor(roomBounds *n);
			void addDoorPosition(FVector &pos);

			void createDoorTo(roomBounds *n);

		private:
			int xScale;
			int yScale;
			int xPos;
			int yPos;

			std::vector<FVector> doorPositions;

			std::vector<roomBounds *> neighbors;
	};

	//inner grid class to save the map
	class grid{
		public:
			grid(int x, int y);
			~grid();
			void fill(int fromX, int fromY, int toX, int toY, layoutCreator::roomBounds *p);
			void add(int x, int y, roomBounds *p);
			bool isFree(int x, int y);
			bool isAreaFree(int x, int y, int x1, int y1);
			bool findAndAdd(layoutCreator::roomBounds *p);

			FString toString();

		private:
			bool isValidIndex(int x, int y);
			TArray<TArray<layoutCreator::roomBounds *>> data;
	};

	//IV
	std::vector<layoutCreator::roomBounds *> created;
	class grid *map = nullptr;

	class UWorld *worldPointer = nullptr;

	int number;

	//IM
	void clean();
	void fillLayout();
	void debugPrintMap();

	void createRoomStartingFromSize(int x, int y);
	roomBounds* testRoom(int x, int y);


	
	std::vector<roomBounds> copyData();
};
