// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include <map>


/**
 * 
 */


class P2_API RoomManager
{
public:
	RoomManager();
	~RoomManager();

	UClass *getBpFor(int xSize, int ySize);

	void add(UWorld *world, UClass *uclass);

private:
	static const int ONE_METER = 100;

	std::map<std::string, UClass *> map;

	std::string createKey(int xSize, int ySize);

	bool contains(int x, int y);
	bool contains(std::string &key);

};
