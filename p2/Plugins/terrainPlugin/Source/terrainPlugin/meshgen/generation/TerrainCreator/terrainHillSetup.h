// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * stores data for creating hills on the terrain, wraps values for better code readability
 */
class TERRAINPLUGIN_API terrainHillSetup
{
public:

	/// @brief pass in the parameters as chunk index space!
	/// @param posX 
	/// @param posY 
	/// @param scaleX 
	/// @param scaleY 
	/// @param minHeightAdd 
	/// @param maxHeightAdd 
	terrainHillSetup(int posX, int posY, int scaleX, int scaleY, int minHeightAdd, int maxHeightAdd);
	terrainHillSetup(const terrainHillSetup &other);
	terrainHillSetup &operator=(const terrainHillSetup &other);

	~terrainHillSetup();

	int xPosCopy();
	int yPosCopy();
	int xTargetCopy();
	int yTargetCopy();

	FVector center();
	

	int getHeightIfSetOrRandomHeight();
	int getHeightIfSetOrRandomHeight(int i, int j, bool useGaussian);

	void forceSetHeight(int heightIn); //forceHeight
	int getForcedSetHeight();

	bool doesOverlapArea(int startX, int startY, int scaleX, int scaleY);

	void extendInEveryDirectionBy(int count);

private:
	int getHeight();
	int evaluateGaussian(int x, int y);

	//(x,y) in chunk index space
	int xPos = 0; //min x
	int yPos = 0; //min x
	int xTarget = 1; //max x
	int yTarget = 1; //max y
	int zMinheightAdd = 100;
	int zMaxheightAdd = 200;

	int forceHeight = 0;
	bool forceHeightWasSet = false;

	bool overlapOnX(int checkvalue);
	bool overlapOnY(int checkvalue);
	bool isInRange(int from, int to, int checkValue);

	bool isEnclousingX(int from, int to);
	bool isEnclousingY(int from, int to);

};
