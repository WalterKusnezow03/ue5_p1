#pragma once

#include "CoreMinimal.h"

class P2_API roomBounds{
		public:
			void updatePosition(int xPos, int yPos);

			//create room type too!, and door positions
			int number;
			roomBounds(int xIn, int yIn, int num, roomtypeEnum typeIn);
			roomBounds(int xIn, int yIn, int num, UClass *uclassIn);
			~roomBounds();

			roomtypeEnum readType();
			UClass *uclass;
			UClass *readBp();

			int xpos();
			int ypos();

			int xscale();
			int yscale();

			int xOuteredge();
			int yOuteredge();

			void addNeighbor(roomBounds *n);
			void addDoorPosition(int x, int y);
			void addWindowPosition(int x, int y);

			//void createDoorTo(roomBounds *n);

			std::vector<FVector> &readRelativeDoorPositions();
			std::vector<FVector> &readRelativeWindowPositions();

		private:
			int xScale;
			int yScale;
			int xPos;
			int yPos;

			/// @brief relative door positions for the room
			std::vector<FVector> doorPositions;

			roomtypeEnum type;

			std::vector<FVector> windowPositions;
};