// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "entities/EntityScript.h"
#include "playerScript.h"

/**
 * 
 */
class P2_API referenceManager
{
public:
	
	~referenceManager();

	static referenceManager *instance();

	void setPlayerReference(AplayerScript *newPointer);
	AplayerScript *getPlayerPointer();


private:
	//private constructor for singleton
	referenceManager();

	static class referenceManager *instancePointer;

	class AplayerScript *playerPointer;

};
