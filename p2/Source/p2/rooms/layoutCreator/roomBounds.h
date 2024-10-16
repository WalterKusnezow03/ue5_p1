#pragma once

#include "CoreMinimal.h"
#include "p2/util/TTouple.h"

class P2_API roomBounds{
	public:
		void updatePosition(int xPos, int yPos);

		/// @brief stays public, just for debugging
		int number;

		roomBounds(const roomBounds &other); //std vec, etc braucht const beim copy constructor!
		roomBounds &operator=(const roomBounds &other);

		//roomBounds(int xIn, int yIn, int num, roomtypeEnum typeIn);
		roomBounds(int xpos, int ypos, int xScaleIn, int yScaleIn, int num);
		roomBounds(int xIn, int yIn, int num, UClass *uclassIn);
		roomBounds(int xIn, int yIn, int num, UClass *uclassIn, int floorIn, roomtypeEnum typeIn);
		~roomBounds();

		std::vector<FVector> relativeDoorPositionsCm();
		std::vector<FVector> relativeWindowPositionsCm();

		roomtypeEnum readType();
	private:
		UClass *uclass; //bp to spawn
	public:
		UClass *readBp();
		void updateBp(UClass *ucl);

		int xpos();
		int ypos();

		int xscale();
		int yscale();

		int xOuteredge();
		int yOuteredge();

		void addNeighbor(roomBounds *n);
		void addDoorPosition(int x, int y);
		void addWindowPosition(int x, int y);
		void addDoorPosition(FVector pos);
		void addWindowPosition(FVector pos);

		void clampLocalPosition(FVector &pos);

		//void createDoorTo(roomBounds *n);
		std::vector<FVector> &readRelativeDoorPositions();
		std::vector<FVector> &readRelativeWindowPositions();

		//staircase custom doors testing
		bool isStaircase();
		TTouple<int, int> getmanualDoorPos();
		TTouple<int, int> getmanualDoorPosFromRight();
		TTouple<int, int> getmanualDoorPosFromTop();



		//new method, room will create doors on its own
		void connectTo(roomBounds *other);
			

	private:
		int xScale;
		int yScale;
		int xPos;
		int yPos;

		std::vector<roomBounds *> neighborRooms;

		/// @brief relative door positions for the room
		std::vector<FVector> doorPositions;

		roomtypeEnum type;

		std::vector<FVector> windowPositions;


		// ----  TESTING AREA ----
		//floor for staircase door positions
		//as staircases will always be just in one dir down to up for now
		//this will work out

		//just floor for now
		int floor; //WHY does thius exist ????


		//new helper methods
		int xMax();
		int yMax();
		void finishConnection(roomBounds *other, FVector doorPos);
	
};