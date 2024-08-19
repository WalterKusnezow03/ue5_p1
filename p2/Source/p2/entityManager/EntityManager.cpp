// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManager.h"
#include "EntityManagerGeneric.h"
#include "Engine/World.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/throwableItems/throwableEnum.h"
#include "p2/throwableItems/throwableItem.h"
#include <map>


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

    
    grenadeBpClass = nullptr;
    rocketBpClass = nullptr;
    molotovBpClass = nullptr;
}

EntityManager::~EntityManager()
{
    entityBpClass = nullptr;
	humanEntityBpClass = nullptr;
	weaponBpClass = nullptr;
    pistolBpClass = nullptr;

    grenadeBpClass = nullptr;
    rocketBpClass = nullptr;
    molotovBpClass = nullptr;
}

/// @brief add an entity to the manager
/// @param entity 
void EntityManager::add(AEntityScript *entity){
    if(entity != nullptr){
        //set location and deactivate
        entity->enableActiveStatus(false);

        entityList.add(entity);
    }
}

/// @brief add an humanentity to the manager
/// @param humanEntity 
void EntityManager::add(AHumanEntityScript *humanEntity){
    if(humanEntity != nullptr){
        humanEntity->enableActiveStatus(false);
        humanEntityList.add(humanEntity);
    }
}

void EntityManager::add(Aweapon *weaponIn){
    if(weaponIn != nullptr){
        weaponEnum type = weaponIn->readType();

        //push in correct vector
        std::map<weaponEnum, EntityManagerGeneric<Aweapon> *> map;
        map[weaponEnum::assaultRifle] = &assault_weaponList;
        map[weaponEnum::pistol] = &pistol_weaponList;

        EntityManagerGeneric<Aweapon> *m = map[type];
        if(m != nullptr){
            m->add(weaponIn);
        }
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








/// @brief spawns a weapon and returns the pointer, applies default attachments
/// @param world world to spawn in
/// @param typeToSpawn weapon to spawn
/// @return weapon reference
Aweapon *EntityManager::spawnAweapon(UWorld* world, weaponEnum typeToSpawn){
    DebugHelper::showScreenMessage("try get weapon");
    FVector Location = FVector(0, 0, 0);

    UClass *selectedBp = weaponBpClass; //to created, default is stick gun

    std::map<weaponEnum, UClass *> map;
    map[weaponEnum::assaultRifle] = assaultRifleBpClass;
    map[weaponEnum::pistol] = pistolBpClass;

    selectedBp = map[typeToSpawn];
    if(selectedBp != nullptr){
        AActor *spawned = spawnAactor(world, selectedBp, Location);
        Aweapon *w = Cast<Aweapon>(spawned);
        if(w != nullptr){
            //testing as default sight
            w->applySight(weaponSightEnum::enum_ironsight);

            return w;
        }
    }
    
    return nullptr;
}


//create weapon from helper object

/// @brief spawns a weapon with an helper object
/// @param world world to spawn in
/// @param helper helper object. CANT BE NULLPTR
/// @return weapon or nullptr
Aweapon* EntityManager::spawnAweapon(UWorld *world, weaponSetupHelper *helper){
    if(helper != nullptr && world != nullptr){
        weaponEnum typeToSpawn = helper->getWeaponTypeToCreate();
        Aweapon *created = spawnAweapon(world, typeToSpawn);

        if(created != nullptr){
            helper->applyAttachments(created);
            return created; //return the created weapon obviously
        }
    }
    return nullptr;
}


/// @brief spawns a throwable item if possible
/// @param world world to spawn in
/// @param location to spawn at
/// @param type type to spawn
/// @return pointer to the AthrowableItem (derived from aactor)
AthrowableItem* EntityManager::spawnAthrowable(UWorld *world, FVector location, throwableEnum type){

    if(world != nullptr){
        std::map<throwableEnum, UClass *> typeMap;

        typeMap[throwableEnum::rock_enum] = nullptr;
        typeMap[throwableEnum::greneade_enum] = grenadeBpClass;
        typeMap[throwableEnum::rocket_enum] = rocketBpClass;
        typeMap[throwableEnum::molotov_enum] = molotovBpClass;

        UClass *fromMap = typeMap[type];
        if(fromMap != nullptr){

            AActor * spawned = spawnAactor(world, fromMap, location);

            AthrowableItem *casted = Cast<AthrowableItem>(spawned);
            if(casted != nullptr){
                return casted;
            }
        }
    }
    //an issue occured
    return nullptr;
}



//testing thrower weapons
Aweapon *EntityManager::spawnAweapon(UWorld* world, throwableEnum typeToSpawn){
    DebugHelper::showScreenMessage("try get weapon");
    FVector Location = FVector(0, 0, 0);

    UClass *selectedBp = nullptr; //to created, default is stick gun

    std::map<throwableEnum, UClass *> map;
    map[throwableEnum::rocket_enum] = rocketBpClass;
    map[throwableEnum::greneade_enum] = grenadeBpClass;

    selectedBp = map[typeToSpawn];
    if(selectedBp != nullptr){
        AActor *spawned = spawnAactor(world, selectedBp, Location);
        Aweapon *w = Cast<Aweapon>(spawned);
        if(w != nullptr){

            return w;
        }
    }

    return nullptr;
}

//testing needed
void EntityManager::setDefaultThrowerClassBp(UClass *uIn){
    if(defaultThrower == nullptr){
        defaultThrower = uIn;
    }
}









/**
 * REFERENCE SETTINGS / SET U CLASS SECTION
 * 
 * 
 */


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



/// @brief sets the weapon blue print of a specific type
/// @param weaponIn 
/// @param typeIn 
void EntityManager::setWeaponUClassBP(UClass *weaponIn, weaponEnum typeIn){
    if(weaponIn != nullptr){
        switch(typeIn){
        case weaponEnum::assaultRifle: assaultRifleBpClass = weaponIn;
            break;
        case weaponEnum::pistol: pistolBpClass = weaponIn;
            break;
        }
    }
    
}


/// @brief sets the throwable item for all throwables if possible
/// @param throwableIn u class derived of throwableItems/throwableItem
/// @param typeIn type of the throwable
void EntityManager::setThrowableUClassBp(UClass *throwableIn, throwableEnum typeIn){
    if(throwableIn != nullptr){
        switch(typeIn){
        case throwableEnum::rocket_enum: rocketBpClass = throwableIn;
            break;
        case throwableEnum::greneade_enum: grenadeBpClass = throwableIn;
            break;
        case throwableEnum::molotov_enum: molotovBpClass = throwableIn;
            break;
        }
    }
}