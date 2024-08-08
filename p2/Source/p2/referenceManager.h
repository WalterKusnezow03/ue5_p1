// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "playerScript.h"

/**
 * 
 */
class P2_API referenceManager
{
public:
	referenceManager();
	~referenceManager();

	static class AplayerScript *playerPointer;

	static void setPlayerReference(AplayerScript *newPointer);
	static AplayerScript *getPlayerPointer();
};
