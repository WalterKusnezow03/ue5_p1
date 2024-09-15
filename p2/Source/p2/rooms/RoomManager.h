// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "p2/rooms/layoutCreator/roomBounds.h"
#include <map>

/**
 * the room manager holds the specific uclasses for the rooms and
 * creates room layouts on request
 */
class P2_API RoomManager
{
public:
	RoomManager();
	~RoomManager();

	UClass *getBpFor(int xSize, int ySize, roomtypeEnum type);

	//room blueprints
	void add(UWorld *world, UClass *uclass);



	void createALayout(UWorld *world, FVector &location, int x, int y);

	bool contains(int x, int y, roomtypeEnum type);


	class sizeData{
		public:
			sizeData(int x, int y, roomtypeEnum typeIn, UClass *uclassIn);
			~sizeData();
			int xSize();
			int ySize();
			std::string createKey();
			UClass *getBp();

		private:
			roomtypeEnum type;
			int xsize;
			int ysize;
			class UClass *uclassBp;
	};

private:
	std::map<roomtypeEnum, std::vector<RoomManager::sizeData>> vectorMap;

	UClass *singleTileBp;

public:
	sizeData *getAny(roomtypeEnum type);

private:
	static const int ONE_METER = 100;
	int zWallHeight = 0;

	int convertMeterToIndex(int a);
	int convertScaleToMeter(int a);
	void convertScaleToMeterFVector(FVector &vector);
	




	void processLayer(UWorld* world, std::vector<roomBounds> &ref, FVector offset, bool createWalls, int &outheight);
};
