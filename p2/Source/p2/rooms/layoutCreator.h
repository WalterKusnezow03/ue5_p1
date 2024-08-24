// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/rooms/room.h"
#include "p2/entityManager/RoomManager.h"
#include <list>

class Aroom;

/**
 * 
 */
class P2_API layoutCreator
{
public:
	layoutCreator(RoomManager *manager);
	~layoutCreator();

	void createRooms(int x, int y);
	

	class roomBounds{
		public:
			void updatePosition(int xPos, int yPos);

			//create room type too!, and door positions
			int number;
			roomBounds(int xIn, int yIn, int num);
			~roomBounds();
			int xpos();
			int ypos();

			int xscale();
			int yscale();

			int xOuteredge();
			int yOuteredge();

			void addNeighbor(roomBounds *n);
			void addDoorPosition(int x, int y);

			void createDoorTo(roomBounds *n);

			std::vector<FVector> &readDoorPositions();

		private:
			int xScale;
			int yScale;
			int xPos;
			int yPos;

			std::vector<FVector> doorPositions;

			//std::vector<roomBounds *> neighbors;
	};

	//public method to copy to room data, might be switched to be by reference
	std::vector<roomBounds> copyData();

private:
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
			bool isValidIndex(int x, int y);
			layoutCreator::roomBounds *tryGetPosition(int x, int y);

			FString toString();

		private:
			/// @brief stores the grid data, will have duplicate pointers
			/// will show if an point and area is used or not
			TArray<TArray<layoutCreator::roomBounds *>> data;

	};

	//room manager reference to ask for valid map sizes
	class RoomManager *manager;

	//std::map<std::string, layoutCreator::roomBounds> sizeMap;

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

	bool canCreate(int x, int y);
	roomBounds *testRoom(int x, int y);

	void connectNeighbors();
};
