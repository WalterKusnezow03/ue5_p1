// Fill out your copyright notice in the Description page of Project Settings.

#include "referenceManager.h"

#include "playerScript.h"
#include "EntityScript.h"
#include "weapon.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"


referenceManager::referenceManager(){
    playerPointer = nullptr;
    weaponBpClass = nullptr;
    entityBpClass = nullptr;
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



/// @brief set the uclass reference for spawning enteties
/// @param entityIn 
void referenceManager::setEntityUClassBp(UClass *entityIn){
    if(entityIn != nullptr){
        entityBpClass = entityIn;
    }
}

/// @brief sets the human entity bp reference
/// @param humanIn 
void referenceManager::setHumanEntityUClassBp(UClass *humanIn){
    if(humanIn != nullptr){
        humanEntityBpClass = humanIn;
    }
}


/// @brief sets the weapon bp reference
/// @param weaponIn 
void referenceManager::setWeaponUClassBp(UClass *weaponIn){
    if(weaponIn != nullptr){
        weaponBpClass = weaponIn;
    }
}


//spawn methods

/// @brief spawns any actor from uclass (from content likely)
/// @param world world to spawn in
/// @param toSpawn actor to spawn
/// @param Location location to spawn at
/// @return spawned actor pointer
AActor *referenceManager::spawnAactor(UWorld * world, UClass *toSpawn, FVector Location){
    if(world != nullptr && toSpawn != nullptr){

        // Initialize SpawnParams if needed
        FActorSpawnParameters SpawnParams;

        // Spawn the actor
        AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams);
        if(spawned != nullptr){
            return spawned;
        }
            
    }
    return nullptr;
}


/// @brief spawns an entity in the world
/// @param world 
/// @param Location 
void referenceManager::spawnEntity(UWorld* world, FVector Location) {
    AActor *actor = spawnAactor(world, entityBpClass, Location);
}


/// @brief spawns an human entity in the world
/// @param world 
/// @param Location 
void referenceManager::spawnHumanEntity(UWorld* world, FVector Location) {
    AActor *actor = spawnAactor(world, humanEntityBpClass, Location);
}

/// @brief spawns a weapon and returns the pointer
/// @param world world to spawn in
/// @return weapon reference
Aweapon *referenceManager::spawnAweapon(UWorld* world){

    FVector Location = FVector(0, 0, 0);
    AActor *spawned = spawnAactor(world, weaponBpClass, Location);
    Aweapon *w = Cast<Aweapon>(spawned);
    if(w != nullptr){
        return w;
    }
    return nullptr;
}