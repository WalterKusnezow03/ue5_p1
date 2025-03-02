// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManager.h"
#include "EntityManagerGeneric.h"
#include "p2/particleSystem/particle.h"
#include "p2/particleSystem/particleEnum.h"
#include "p2/meshgen/customMeshActor.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "p2/meshgen/water/customWaterActor.h"
#include "p2/gamestart/assetManager.h"

#include "Engine/World.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/throwableItems/throwableEnum.h"
#include "p2/throwableItems/throwableItem.h"
#include "p2/util/FVectorUtil.h"

#include <map>



EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
    
}


template class EntityManagerGeneric<AEntityScript>;
template class EntityManagerGenericMap<teamEnum, AHumanEntityScript>;
template class EntityManagerGeneric<AcustomMeshActor>;
template class EntityManagerGenericMap<weaponEnum, Aweapon>;
template class EntityManagerGenericMap<throwableEnum, AthrowableItem>;
template class EntityManagerGenericMap<particleEnum, Aparticle>;



/// @brief add an entity to the manager
/// @param entity 
void EntityManager::add(AEntityScript *entity){
    if(entity != nullptr){
        //set location and deactivate
        entity->enableActiveStatus(false);

        entityList.add(entity);
    }
}

/// @brief add an humanentity to the manager and hides it
/// @param humanEntity 
void EntityManager::add(AHumanEntityScript *humanEntity){
    if(humanEntity != nullptr){
        humanEntity->enableActiveStatus(false);

        //humanEntityList.add(humanEntity);
        humanEntityMap.add(humanEntity->getTeam(), humanEntity);
    }
}

/// @brief adds a weapon to the entity manager, and hides it
/// @param weaponIn 
void EntityManager::add(Aweapon *weaponIn){
    
    if(weaponIn != nullptr){
        FVector hiddenLocation = FVector(0, 0, -1000);
        weaponIn->SetActorLocation(hiddenLocation);


        weaponIn->showWeapon(false);
        weaponEnum type = weaponIn->readType();

        //new map manager
        weaponMap.add(type, weaponIn);

    }
}

/// @brief adds a throwable to the entity manager
/// @param throwableItem 
void EntityManager::add(AthrowableItem *throwableItem){
    if(throwableItem != nullptr){
        throwableEnum type = throwableItem->getType();
        throwableMap.add(type, throwableItem);
    }
}


/// @brief adds a partile to the entity manager
/// @param particleIn 
void EntityManager::add(Aparticle *particleIn){
    if(particleIn != nullptr){
        //particle will manage this it self
        //AActorUtil::showActor(*particleIn, false);
        particleEnum type = particleIn->getType();
        particleMap.add(type, particleIn);
    }
}

/// @brief adds a mesh actor the entity manager
/// @param meshActorIn 
void EntityManager::add(AcustomMeshActor *meshActorIn){
    if(meshActorIn != nullptr){
        meshActorList.add(meshActorIn);
    }
}



/**
 * ----- RAYCAST PARAM SECTION -----
 */

FCollisionQueryParams &EntityManager::getIgnoredRaycastParams(){
    return collisionIgnoreParams;
}


FCollisionQueryParams &EntityManager::getIgnoredRaycastParams(teamEnum team){
    return collisionMap[team];
    // return collisionIgnoreParams;
}

/// @brief adds a actor to the ignored params which are used by entiteies for raycasting and 
/// pathfinder
/// improoves performance
/// @param actor actor reference to be ignored in own team and all ignored actors (they are seperate params)
void EntityManager::addActorToIgnoreRaycastParams(AActor *actor, teamEnum team){
    if(actor != nullptr){
        //add to correct map
        FCollisionQueryParams *ref = &collisionMap[team];
        ref->AddIgnoredActor(actor);

        //&collisionMap[team].AddIgnoredActor(actor); // add to team map

        //add to player and enemy team if neutral to create a proper filter
        if(team == teamEnum::neutralTeam || team == teamEnum::none){
            ref = &collisionMap[teamEnum::playerTeam];
            ref->AddIgnoredActor(actor);
            ref = &collisionMap[teamEnum::enemyTeam];
            ref->AddIgnoredActor(actor);
        }

        //add to all
        addActorToIgnoredAllParams(actor);
    }
}

/// @brief adds a ignored actor to the all ignore params
/// @param actor actor to ignore
void EntityManager::addActorToIgnoredAllParams(AActor *actor){
    if(actor != nullptr){
        collisionIgnoreParams.AddIgnoredActor(actor);
    }
}







/**
 * ---- SPAWN SECTION HERE ----
 */


/// @brief spawns an entity in the world
/// @param world 
/// @param Location 
AEntityScript* EntityManager::spawnEntity(UWorld* world, FVector &Location) {

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
    if(assetManager *a = assetManager::instance()){
        UClass *bp = a->findBp(entityEnum::entity_enum);
        if(bp != nullptr){
            AActor *actor = spawnAactor(world, bp, Location);
            if(actor != nullptr){
                addActorToIgnoreRaycastParams(actor, teamEnum::neutralTeam);
                AEntityScript *casted = Cast<AEntityScript>(actor);
                if(casted != nullptr){
                    return casted;
                }
            }
        }
    }

    /*
    AActor *actor = spawnAactor(world, entityBpClass, Location);
    AEntityScript *casted = Cast<AEntityScript>(actor);
    if(casted != nullptr){
        return casted;
    }*/
    return nullptr;
}

/// @brief spawns an human entity in the world
/// @param world 
/// @param Location 
AHumanEntityScript* EntityManager::spawnHumanEntity(UWorld* world, FVector &Location, teamEnum team) {

    //get from list if any left
    if(humanEntityMap.hasActorsLeft(team)){

    //if(humanEntityList.hasActorsLeft()){
    //    AHumanEntityScript *human = humanEntityList.getFirstActor();
        AHumanEntityScript *human = humanEntityMap.getFirstActor(team);
        if(human != nullptr){
            //DebugHelper::showScreenMessage("human from list !", FColor::Yellow);

            human->init();
            human->SetActorLocation(Location);

            return human;
        }
    }

    
   if(assetManager *a = assetManager::instance()){
        UClass *bp = a->findBp(entityEnum::human_enum);
        if(bp != nullptr){
            AActor *actor = spawnAactor(world, bp, Location);
            if(actor != nullptr){
                addActorToIgnoreRaycastParams(actor, team);
                AHumanEntityScript *casted = Cast<AHumanEntityScript>(actor);
                if(casted != nullptr){
                    casted->init();
                    casted->setTeam(team);
                    return casted;
                }
            }
        }
    }
    return nullptr;
}




/// @brief spawns any actor from uclass (from content likely)
/// @param world world to spawn in
/// @param toSpawn actor to spawn
/// @param Location location to spawn at
/// @return spawned actor pointer
AActor *EntityManager::spawnAactor(UWorld * world, UClass *toSpawn, FVector &Location){
    if(world != nullptr && toSpawn != nullptr){
        
        //check if the type to spawn is even aactor and the casting is valid
        //other wise things get messed up and different points created
        if(toSpawn->IsChildOf(AActor::StaticClass())){ 
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams);
            if(spawned != nullptr){
                return spawned;
            }
        }
       
            
    }
    return nullptr;
}








/// @brief spawns a weapon and returns the pointer, applies default attachments
/// @param world world to spawn in
/// @param typeToSpawn weapon to spawn
/// @return weapon reference
Aweapon *EntityManager::spawnAweapon(UWorld* world, weaponEnum typeToSpawn){
    
    FVector Location = FVector(0, 0, 0);

    //weapon map testing
    if(weaponMap.hasActorsLeft(typeToSpawn)){
        Aweapon *fromManager = weaponMap.getFirstActor(typeToSpawn);
        if(fromManager != nullptr){
            fromManager->showItem(true);

            //testing as default sight
            fromManager->applySight(weaponSightEnum::enum_ironsight);
            return fromManager;
        }
    }

    UClass *selectedBp = nullptr;
    if(assetManager *a = assetManager::instance()){
        selectedBp = a->findBp(typeToSpawn);
    }

    if(selectedBp != nullptr){
        AActor *spawned = spawnAactor(world, selectedBp, Location);
        if(spawned != nullptr){
            addActorToIgnoreRaycastParams(spawned, teamEnum::neutralTeam);
        }
        Aweapon *w = Cast<Aweapon>(spawned);
        if(w != nullptr){
            //show weapon
            w->showItem(true);

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
AthrowableItem* EntityManager::spawnAthrowable(UWorld *world, FVector &location, throwableEnum type){

    if(world != nullptr){

        //DebugHelper::showScreenMessage("THROWABLE REQUETS TEST");
        if (throwableMap.hasActorsLeft(type))
        {
            //DebugHelper::showScreenMessage("THROWABLE REQUEST OK ", FColor::Green);
            AthrowableItem *a = throwableMap.getFirstActor(type);
            if(a != nullptr){
                a->reset();
                return a;
            }
        }


        UClass *fromMap = nullptr;
        if(assetManager *a = assetManager::instance()){
            fromMap = a->findBp(type);
        }

        if(fromMap != nullptr){

            AActor * spawned = spawnAactor(world, fromMap, location);
            if(spawned != nullptr){
                addActorToIgnoreRaycastParams(spawned, teamEnum::neutralTeam);
            }

            AthrowableItem *casted = Cast<AthrowableItem>(spawned);
            if(casted != nullptr){
                return casted;
            }
        }
    }
    //an issue occured
    return nullptr;
}



/// @brief spawns a thrower weapon witht the desired throwable if possible
/// @param world 
/// @param typeToSpawn 
/// @return 
Aweapon *EntityManager::spawnAweapon(UWorld* world, throwableEnum typeToSpawn){
    DebugHelper::showScreenMessage("try get weapon");
    FVector Location = FVector(0, 0, 0);

    //new code not tested
    //get from manager or spawn
    AthrowerWeapon *weapon = nullptr;
    if (weaponMap.hasActorsLeft(weaponEnum::thrower))
    {
        Aweapon *fromMap = weaponMap.getFirstActor(weaponEnum::thrower);
        weapon = Cast<AthrowerWeapon>(fromMap);
    }
    else
    {
        UClass *selectedBp = nullptr;
        if(assetManager *a = assetManager::instance()){
            selectedBp = a->findBp(weaponEnum::thrower);
        }
        AActor *spawned = spawnAactor(world, selectedBp, Location);
        weapon = Cast<AthrowerWeapon>(spawned);
    }

    if(weapon != nullptr){
        weapon->setThrowableType(typeToSpawn);
        return weapon;
    }
    return nullptr;
}

/// @brief spawns a custom mesh actor at a given location without any mesh.
/// @param world world to spawn in
/// @param location lcoation to spawn at
/// @return custom mesh actor on success, or nullptr if not
AcustomMeshActor *EntityManager::spawnAcustomMeshActor(UWorld *world, FVector &location){
    if(world != nullptr){

        if(meshActorList.hasActorsLeft()){
            AcustomMeshActor *pointer = meshActorList.getFirstActor();
            if(pointer != nullptr){
                //debug, need to remove later
                //DebugHelper::showScreenMessage("POPPED MESH FROM BACKUP", FColor::Green);
                pointer->SetActorLocation(location);
                return pointer;
            }
        }

        // --- new testing spawn by static class ---
        // Spawn the actor
        FRotator rotation;
        FActorSpawnParameters params;
        AcustomMeshActor *SpawnedActor = world->SpawnActor<AcustomMeshActor>(
            AcustomMeshActor::StaticClass(),
            location,
            FRotator::ZeroRotator,
            params
        );
        return SpawnedActor;

        /*
        if(emptyCustomMeshActorBp != nullptr){
            AActor *actor = spawnAactor(world, emptyCustomMeshActorBp, location);
            if(actor != nullptr){

                AcustomMeshActor *customMesh = Cast<AcustomMeshActor>(actor);
                if(customMesh != nullptr){
                    return customMesh;
                }
            }
        }*/
    }
    return nullptr;
}


/** 
* ---- PARTICLE / EXPLOSION SECTION ----
*/


/// @brief creates an visual explosion at a given location
/// @param world 
/// @param location 
void EntityManager::createExplosion(UWorld *world, FVector &location){
    if(world != nullptr){
        int amount = 20;
        float speed = 200.0f; //2 * 100 cm/s = 2ms

        float smokeLifeTime = 20.0f;
        float fireLifeTime = 1.0f;

        for (int i = 0; i < amount; i++)
        {
            FVector dir = FVectorUtil::randomOffset(100); //lets say just one meter because its normalized
            dir.Z = 200;

            createParticle(world, particleEnum::smoke_enum, location, dir, speed, smokeLifeTime);


            //fire testing
            if(i < 10){
                createParticle(world, particleEnum::fire_enum, location, dir, speed * 10, fireLifeTime);
            }
        }
    }
}


void EntityManager::createDebree(UWorld *world, FVector &location, materialEnum materialType){

    assetManager *am = assetManager::instance();
    if(am != nullptr){
        UMaterial *material = am->findMaterial(materialType);
        if(material != nullptr){
            FVector dir = FVectorUtil::randomOffset(100);
            float speed = 3000.0f;
            int lifetime = 2;
            createParticle(world, material, location, dir, speed, lifetime);
        }
    }
}


/// @brief creates an indivudual particle from an enum type
/// @param world to spawn in
/// @param enumtype type of particle
/// @param location to spawn at
/// @param dir direction of impulse
/// @param speed speed to apply
void EntityManager::createParticle(
    UWorld *world, 
    particleEnum enumtype, 
    FVector &location, 
    FVector &dir, 
    float speed, 
    float lifeTime
){
    
    if(world != nullptr){
        UClass *bp = nullptr; // getParticleBp(enumtype);

        if(assetManager *am = assetManager::instance()){
            bp = am->findBp(enumtype);
        }

        if(bp != nullptr){

            AActor *a = nullptr;
            if(particleMap.hasActorsLeft(enumtype)){
                a = particleMap.getFirstActor(enumtype);
            }else{
                a = spawnAactor(world, bp, location);
            }

            //spawn aactor if none was found
            //AActor *a = spawnAactor(world, bp, location);
            if(a != nullptr){
                a->SetActorLocation(location);
                Aparticle *created = Cast<Aparticle>(a);
                if(created != nullptr){
                    created->setParticleType(enumtype); //set the partcle type on start
                    created->applyImpulse(dir, speed, lifeTime);
                }
            }
        }
    }
    
}


/// @brief creates an indivudual particle from an enum type
/// @param world to spawn in
/// @param UMaterial material to apply
/// @param location to spawn at
/// @param dir direction of impulse
/// @param speed speed to apply
void EntityManager::createParticle(
    UWorld *world, 
    UMaterial *materialToApply,
    FVector &location, 
    FVector &dir, 
    float speed, 
    float lifeTime
){
    
    if(world != nullptr){
        UClass *bp = nullptr; // getParticleBp(enumtype);

        if(assetManager *am = assetManager::instance()){
            bp = am->findBp(particleEnum::particleNone_enum); //none here.
        }

        if(bp != nullptr){

            AActor *a = nullptr;
            if(particleMap.hasActorsLeft(particleEnum::particleNone_enum)){
                a = particleMap.getFirstActor(particleEnum::particleNone_enum);
            }else{
                a = spawnAactor(world, bp, location);
            }

            //spawn aactor if none was found
            //AActor *a = spawnAactor(world, bp, location);
            if(a != nullptr){
                a->SetActorLocation(location);
                Aparticle *created = Cast<Aparticle>(a);
                if(created != nullptr){
                    created->setParticleType(particleEnum::particleNone_enum); //set the partcle type on start

                    FVector scale(0.5f, 0.5f, 0.5f);
                    created->applyImpulse(dir, speed, lifeTime, materialToApply, scale);
                }
            }
        }
    }
    
}







/**
 * ---- SECTION FOR TERRAIN ----
 * 
 */




/// @brief returns a vector by value of meshactor pointers
/// @param world world to spawn in
/// @param requestCount request count how many you need
/// @return vector of actors
std::vector<AcustomMeshActor*> EntityManager::requestMeshActors(UWorld *world, int requestCount){
    std::vector<AcustomMeshActor *> output;
    if (world != nullptr){
        output.reserve(requestCount);
        FVector location(0, 0, 0);

        //fill the vector with actors.
        while (output.size() < requestCount)
        {
            AcustomMeshActor *pointer = spawnAcustomMeshActor(world, location);
            if(pointer != nullptr){
                output.push_back(pointer);
            }
        }
    }

    return output;
}



void EntityManager::createTwoSidedQuad(UWorld *world, FVector &a, FVector &b, FVector &c, FVector &d){
    //implementation needs to be tested!

    if(assetManager *am = assetManager::instance()){
        FVector location(0, 0, 0);

        AcustomMeshActor *customMesh = spawnAcustomMeshActor(world, location);
        if(customMesh != nullptr){

            customMesh->createTwoSidedQuad(
                a, b, c, d,
                materialEnum::wallMaterial
            );

        }

    }
}



