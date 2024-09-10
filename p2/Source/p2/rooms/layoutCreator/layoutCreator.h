// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/rooms/room.h"
#include "roomBounds.h"
#include "p2/util/TTouple.h"
#include "p2/util/TVector.h"
#include "p2/rooms/RoomManager.h"

class Aroom;
class RoomManager;

/**
 * 
 */
class P2_API layoutCreator
{
public:
	layoutCreator(RoomManager *manager);
	~layoutCreator();

	void createRooms(int x, int y, int staircases);
	void createRooms(int x, int y, std::vector<roomBounds> staircases, bool leaveGap);
	void createRooms(int x, int y, std::vector<roomBounds> staircases, bool leaveGap, std::vector<TTouple<int,int>> &block);

	//public method to copy to room data, might be switched to be by reference
	std::vector<roomBounds> copyData();
	std::vector<roomBounds> copyStaircaseData();
	std::vector<TTouple<int,int>> getInverseBlockList();
	std::vector<roomBounds> getRoofToCreate(UClass *singleTile);

private:
	//inner grid class to save the map
	class grid{
		public:
			grid(int x, int y);
			~grid();
			void fill(int fromX, int fromY, int toX, int toY, roomBounds *p);
			void add(int x, int y, roomBounds *p);
			bool isFree(int x, int y);
			bool isAreaFree(int x, int y, int x1, int y1);
			bool areaHasAtLeastOneNeighbor(int x, int y, int x1, int y1);
			bool areaIsValid(int x, int y, int x1, int y1);
			bool findFreeArea(int xSize, int ySize, int &outX, int &outY);

			
			void forceAdd(roomBounds *p);
			void forceAdd(roomBounds *p, int x, int y);

			bool isValidIndex(int x, int y);
			roomBounds *tryGetPosition(int x, int y);

			FString toString();

			//void getEdges(roomBounds &roomToCheck, std::vector<TTouple<int,int>> &output, int &size);
			void getEdges(roomBounds &roomToCheck, TVector<TTouple<int,int>> &output);

			void getValidWindowPositions(
				int x1, int y1, int x2, int y2, // offset must be applied as wanted
				TVector<TTouple<int, int>> &output
			);

			void fillInverseBlock(std::vector<TTouple<int, int>> &output);
			void fillEmptyGapsRoof(std::vector<TTouple<int, int>> &output);

		private:
			/// @brief stores the grid data, will have duplicate pointers
			/// will show if an point and area is used or not
			TArray<TArray<roomBounds *>> data;

	};

	//saves the staircases to create
	int staircasesLeft;

	//room manager reference to ask for valid map sizes
	class RoomManager *manager;

	//std::map<std::string, layoutCreator::roomBounds> sizeMap;

	//IV
	std::vector<roomBounds *> created;
	std::vector<roomBounds *> createdStaircases;
	class grid *map = nullptr;

	class UWorld *worldPointer = nullptr;

	int number;

	//IM
	void clean();
	void debugPrintMap();
	void connectNeighbors();
	void createWindows();

	void tryCreateRooms();
	bool createRoom(roomtypeEnum type);

	template <typename T>
	void removeRandomElements(TVector<T> &t, float fractionToKeep);
	

	//dummy room
	static class roomBounds *dummy;
};
