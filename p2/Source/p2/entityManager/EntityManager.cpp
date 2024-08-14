// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManager.h"
#include "EntityManagerGeneric.h"
#include "Engine/World.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"


//set static ref to nullptr
EntityManager *EntityManager::instancePointer = nullptr;

EntityManager* EntityManager::instance(){
    if(EntityManager::instancePointer == nullptr){
        instancePointer = new EntityManager();
    }
    return instancePointer;
}

EntityManager::EntityManager()
{
    entityBpClass = nullptr;
	humanEntityBpClass = nullptr;
	weaponBpClass = nullptr;
    pistolBpClass = nullptr;
}

EntityManager::~EntityManager()
{
    entityBpClass = nullptr;
	humanEntityBpClass = nullptr;
	weaponBpClass = nullptr;
    pistolBpClass = nullptr;
}


void EntityManager::add(AEntityScript *entity){
    if(entity != nullptr){
        //set location and deactivate
        entity->enableActiveStatus(false);

        entityList.add(entity);
    }
}

void EntityManager::add(AHumanEntityScript *humanEntity){
    if(humanEntity != nullptr){
        humanEntity->enableActiveStatus(false);
        humanEntityList.add(humanEntity);
    }
}




/// @brief spawns an entity in the world
/// @param world 
/// @param Location 
AEntityScript* EntityManager::spawnEntity(UWorld* world, FVector Location) {

    //get from list if any left
    if(entityList.hasActorsLeft()){
        AEntityScript *entity = entityList.getFirstActor();
        if(entity != nullptr){
            entity->init();
            entity->SetActorLocation(Location);
            return entity;
        }
    }

    //else: create
    AActor *actor = spawnAactor(world, entityBpClass, Location);
    AEntityScript *casted = Cast<AEntityScript>(actor);
    if(casted != nullptr){
        return casted;
    }
    return nullptr;
}

/// @brief spawns an human entity in the world
/// @param world 
/// @param Location 
AHumanEntityScript* EntityManager::spawnHumanEntity(UWorld* world, FVector Location) {

    //get from list if any left
    if(humanEntityList.hasActorsLeft()){
        AHumanEntityScript *human = humanEntityList.getFirstActor();
        if(human != nullptr){
            human->init();
            human->SetActorLocation(Location);
            return human;
        }
    }

    

    //else create new one
    AActor *actor = spawnAactor(world, humanEntityBpClass, Location);
    AHumanEntityScript *casted = Cast<AHumanEntityScript>(actor);
    if(casted != nullptr){
        DebugHelper::showScreenMessage("try spawn human");
        return casted;
    }
    return nullptr;
}




/// @brief spawns any actor from uclass (from content likely)
/// @param world world to spawn in
/// @param toSpawn actor to spawn
/// @param Location location to spawn at
/// @return spawned actor pointer
AActor *EntityManager::spawnAactor(UWorld * world, UClass *toSpawn, FVector Location){
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








/// @brief spawns a weapon and returns the pointer
/// @param world world to spawn in
/// @return weapon reference
Aweapon *EntityManager::spawnAweapon(UWorld* world){
    DebugHelper::showScreenMessage("try get weapon");
    FVector Location = FVector(0, 0, 0);
    AActor *spawned = spawnAactor(world, weaponBpClass, Location);
    Aweapon *w = Cast<Aweapon>(spawned);
    if(w != nullptr){
        return w;
    }
    return nullptr;
}




//REFERENCE SETTINGS


/// @brief set the uclass reference for spawning enteties
/// @param entityIn 
void EntityManager::setEntityUClassBp(UClass *entityIn){
    if(entityIn != nullptr){
        entityBpClass = entityIn;
    }
}

/// @brief sets the human entity bp reference
/// @param humanIn 
void EntityManager::setHumanEntityUClassBp(UClass *humanIn){
    if(humanIn != nullptr){
        humanEntityBpClass = humanIn;
    }
}


/// @brief sets the weapon bp reference
/// @param weaponIn 
void EntityManager::setWeaponUClassBp(UClass *weaponIn){
    if(weaponIn != nullptr){
        weaponBpClass = weaponIn;
    }
}


void EntityManager::setPistolUClassBp(UClass *pistolIn){
    if(pistolIn != nullptr){
        pistolBpClass = pistolIn;
    }
}
