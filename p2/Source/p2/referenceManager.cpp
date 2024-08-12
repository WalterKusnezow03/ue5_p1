// Fill out your copyright notice in the Description page of Project Settings.

#include "referenceManager.h"

#include "playerScript.h"
#include "entities/EntityScript.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"


referenceManager::referenceManager(){
    playerPointer = nullptr;
}

/// @brief static reference for the singleton
referenceManager *referenceManager::instancePointer = nullptr;



referenceManager::~referenceManager()
{
}

/// @brief returns the instance of the ref manager
/// @return instance to call methods on
referenceManager* referenceManager::instance(){
    if(referenceManager::instancePointer == nullptr){
        referenceManager::instancePointer = new referenceManager();
    }
    return instancePointer;
}

/// @brief set the player reference for enteties which interact with the player, for example entites
/// @param newPointer 
void referenceManager::setPlayerReference(AplayerScript *newPointer){
    if(newPointer != nullptr){
        playerPointer = newPointer;
    }
}

/// @brief returns the player pointer is existent
/// @return current player reference
AplayerScript *referenceManager::getPlayerPointer(){
    return playerPointer;
}

