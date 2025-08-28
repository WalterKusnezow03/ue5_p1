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
	

	

	static referenceManager *instance();
	/// @brief deletes the reference manager instance
	static void EndPlay();

	void setPlayerReference(AplayerScript *newPointer);
	AplayerScript *getPlayerPointer();


	static void showPlayerCursor(bool show);
	
	static void reloadPlayerLoadoutIfNeeded(LoadoutHelper &other);

	static UWorld *GetWorld();

private:
	// private constructor for singleton
	referenceManager();
	~referenceManager();

	static class referenceManager *instancePointer;

	class AplayerScript *playerPointer = nullptr;

};
