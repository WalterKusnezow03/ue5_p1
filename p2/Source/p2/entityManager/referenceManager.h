// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "p2/player/playerScript.h"

/**
 * stores team constants and the player reference which is needed for all the entites
 */
class P2_API referenceManager
{
public:
	//DEPRECATED
	static const int TEAM_NEUTRAL = 0;
	static const int TEAM_ENEMY = 1;
	static const int TEAM_PLAYER = 2;
	static int verifyTeam(int teamIn);
	//DEPRECATED

	~referenceManager();

	static referenceManager *instance();

	void setPlayerReference(AplayerScript *newPointer);
	AplayerScript *getPlayerPointer();

	FVector playerLocation();
	FVector playerLookDir();

private:
	// private constructor for singleton
	referenceManager();

	static class referenceManager *instancePointer;

	class AplayerScript *playerPointer = nullptr;

};
