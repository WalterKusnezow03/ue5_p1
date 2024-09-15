// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class AOutpost;
class AHumanEntityScript;

/**
 * 
 */
class P2_API OutpostManager
{
public:
	static OutpostManager *instance();
	OutpostManager();
	~OutpostManager();

	AOutpost *requestOutpost(UWorld *world, FVector &ownLocation);
	void tryRequestOutpost(UWorld *world, AHumanEntityScript *human);

private:
	static class OutpostManager *instancePointer;

	std::vector<AOutpost *> outposts; //will be cleared by unreal engine

	AOutpost *createOutpost(UWorld *world, FVector &Location);
	AOutpost *nearestOutpostTo(FVector location); //passed by value because of fVector::dist
};
