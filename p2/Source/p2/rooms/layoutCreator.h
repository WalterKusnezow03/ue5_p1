// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/rooms/room.h"
#include "p2/rooms/RoomManager.h"
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
	//saves the room data to spawn a room appropiatly
	class roomBounds{
		public:
			void updatePosition(int xPos, int yPos);

			//create room type too!, and door positions
			int number;
			roomBounds(int xIn, int yIn, int num, roomtypeEnum typeIn);
			~roomBounds();

			roomtypeEnum readType();

			int xpos();
			int ypos();

			int xscale();
			int yscale();

			int xOuteredge();
			int yOuteredge();

			void addNeighbor(roomBounds *n);
			void addDoorPosition(int x, int y);

			//void createDoorTo(roomBounds *n);

			std::vector<FVector> &readRelativeDoorPositions();

		private:
			int xScale;
			int yScale;
			int xPos;
			int yPos;

			/// @brief relative door positions for the room
			std::vector<FVector> doorPositions;

			roomtypeEnum type;

			//std::vector<roomBounds *> neighbors;
	};

	void createRooms(int x, int y, int staircases);
	void createRooms(int x, int y, std::vector<layoutCreator::roomBounds> staircases);

	//public method to copy to room data, might be switched to be by reference
	std::vector<layoutCreator::roomBounds> copyData();
	std::vector<layoutCreator::roomBounds> copyStaircaseData();

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
			void forceAdd(layoutCreator::roomBounds *p);

			bool isValidIndex(int x, int y);
			layoutCreator::roomBounds *tryGetPosition(int x, int y);

			FString toString();

		private:
			/// @brief stores the grid data, will have duplicate pointers
			/// will show if an point and area is used or not
			TArray<TArray<layoutCreator::roomBounds *>> data;

	};

	//saves the staircases to create
	int staircasesLeft;

	//room manager reference to ask for valid map sizes
	class RoomManager *manager;

	//std::map<std::string, layoutCreator::roomBounds> sizeMap;

	//IV
	std::vector<layoutCreator::roomBounds *> created;
	std::vector<layoutCreator::roomBounds *> createdStaircases;
	class grid *map = nullptr;

	class UWorld *worldPointer = nullptr;

	int number;

	//IM
	void clean();
	void fillLayout();
	void debugPrintMap();

	void createRoomStartingFromSize(int x, int y);

	bool canCreate(int x, int y, roomtypeEnum typeIn);
	roomBounds *testRoom(int x, int y);

	void connectNeighbors();
};
