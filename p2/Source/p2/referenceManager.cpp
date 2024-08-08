// Fill out your copyright notice in the Description page of Project Settings.


#include "playerScript.h"
#include "referenceManager.h"

referenceManager::referenceManager()
{
}

/// @brief static player pointer set to nullptr on init
AplayerScript* referenceManager::playerPointer = nullptr;


referenceManager::~referenceManager()
{
}



void referenceManager::setPlayerReference(AplayerScript *newPointer){
    if(newPointer != nullptr){
        referenceManager::playerPointer = newPointer;
    }
}


AplayerScript *referenceManager::getPlayerPointer(){
    return referenceManager::playerPointer;
}