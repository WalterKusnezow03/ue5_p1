// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathFinder.h"

///@brief priority list, deprecated. Adds a node in O(n), ruins pathfinder runtime.
class PATHFINDER_API priorityList
{
public:
	priorityList();
	~priorityList();

	void add(APathFinder::Node *node);
	APathFinder::Node *popLowestFx();

	bool hasNodes();

	

private:
	
	std::vector<APathFinder::Node *> nodes;

};
