// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManager.h"
#include "GameCore/EntityGC/EntityManagerGeneric.h"
#include "p2/particleSystem/particle.h"
#include "terrainPlugin/meshgen/customMeshActor.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "terrainPlugin/meshgen/water/customWaterActor.h"
#include "AssetPlugin/gamestart/assetManager.h"

#include "Engine/World.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/throwableItems/throwableItem.h"
#include "GameCore/util/FVectorUtil.h"
#include "GameCore/DebugHelper.h"
#include "PathFinder/pathFinding/PathFinder.h"
#include "p2/weapon/throwerWeapon.h"

#include "GcGameCore/Launcher/GcLauncher.h"

#include "IkHumanoidModell/SharedRaycastParams/SharedRaycastParamManager.h"

#include <map>


template class EntityManagerGeneric<AEntityScript>;
template class EntityManagerGenericMap<teamEnum, AHumanEntityScript>;
template class EntityManagerGeneric<AcustomMeshActor>;
template class EntityManagerGenericMap<weaponEnum, Aweapon>;
template class EntityManagerGenericMap<throwableEnum, AthrowableItem>;
template class EntityManagerGenericMap<particleEnum, Aparticle>;



//must be called before anything happens. On Begin Play !
void EntityManager::BeginPlay(){
    if(instancePtr != nullptr){
        EndPlay();
    }
    EntityManagerBase::instancePtr = new EntityManager();
}

EntityManager *EntityManager::instance(){
    if(EntityManagerBase::instancePtr == nullptr){
        BeginPlay();
    }
    //gefährlicher code.
    if(EntityManagerBase::instancePtr != nullptr){
        EntityManager *casted = static_cast<EntityManager*>(EntityManagerBase::instancePtr);
        if(casted){
            return casted;
        }
    }
    
    return nullptr;
}




EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
    
}


/**
 * ------ GC ADD SECTION ---------
 */

/// @brief add an entity to the manager
/// @param entity 
void EntityManager::add(AEntityScript *entity){
    if(entity != nullptr){
        //set location and deactivate
        entity->enableActiveStatus(false);

        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            gc->collection.Add<teamEnum>(entity, entity->getTeam());
        }

        //deprecated
        //entityList.add(entity);
    }
}

/// @brief add an humanentity to the manager and hides it
/// @param humanEntity 
void EntityManager::add(AHumanEntityScript *humanEntity){
    if(humanEntity != nullptr){
        humanEntity->enableActiveStatus(false);

        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            gc->collection.Add<teamEnum>(humanEntity, humanEntity->getTeam());
        }
        
        //deprecated
        //humanEntityMap.add(humanEntity->getTeam(), humanEntity);
    }
}

/// @brief adds a weapon to the entity manager, and hides it
/// @param weaponIn 
void EntityManager::add(Aweapon *weaponIn){
    
    if(weaponIn != nullptr){
        FVector hiddenLocation = FVector(0, 0, -1000);
        weaponIn->SetActorLocation(hiddenLocation);


        weaponIn->showWeapon(false);
        
        
        
        
        weaponEnum type = weaponIn->weaponType();
        
        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add<weaponEnum>(weaponIn, type);
        }

        //deprecated
        /*
        //new map manager
        weaponMap.add(type, weaponIn);
        */
    }
}

/// @brief adds a throwable to the entity manager
/// @param throwableItem 
void EntityManager::add(AthrowableItem *throwableItem){
    if(throwableItem != nullptr){
        throwableEnum type = throwableItem->getType();
        //throwableMap.add(type, throwableItem);


        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add<throwableEnum>(throwableItem, type);
        }
    }
}


/// @brief adds a partile to the entity manager
/// @param particleIn 
void EntityManager::add(Aparticle *particleIn){
    if(particleIn != nullptr){
        //particle will manage this it self
        //AActorUtil::showActor(*particleIn, false);
        particleEnum type = particleIn->getType();
        //particleMap.add(type, particleIn);

        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add<particleEnum>(particleIn, type);
        }
    }
}

/// @brief adds a mesh actor the entity manager
/// @param meshActorIn 
void EntityManager::add(AcustomMeshActor *meshActorIn){
    if(meshActorIn != nullptr){
        //meshActorList.add(meshActorIn);


        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add(meshActorIn);
        }
    }
}






/**
 * ----- RAYCAST PARAM SECTION -----
 */

FCollisionQueryParams EntityManager::getIgnoredRaycastParams(){
    //return collisionIgnoreParams;

    //merge with skelleton collision params
    const CollisionTracker *tracker = ASharedRaycastParamManager::getShared();
    if(tracker){
        return tCollisionMap.getAllCollisionParamsMergedWith(*tracker);
    }
    return tCollisionMap.getAllCollisonParams();
}

FCollisionQueryParams EntityManager::getIgnoredRaycastParams(teamEnum team){
    //return collisionMap[team];
    return tCollisionMap.getCollisonParams(team);
}

/// @brief adds a actor to the ignored params which are used by entiteies for raycasting and 
/// pathfinder
/// improoves performance
/// @param actor actor reference to be ignored in own team and all ignored actors (they are seperate params)
void EntityManager::addActorToIgnoreRaycastParams(AActor *actor, teamEnum team){
    if(actor != nullptr){

        tCollisionMap.AddIgnoredActor(team, actor);


        /*
        if(collisionMap.find(team) == collisionMap.end()){
            collisionMap[team] = FCollisionQueryParams();
        }

        //add to correct map
        FCollisionQueryParams *ref = &collisionMap[team];
        ref->AddIgnoredActor(actor);

        //&collisionMap[team].AddIgnoredActor(actor); // add to team map

        //add to player and enemy team if neutral to create a proper filter
        if(team == teamEnum::neutralTeam || team == teamEnum::none){

            if(collisionMap.find(teamEnum::playerTeam) == collisionMap.end()){
                collisionMap[teamEnum::playerTeam] = FCollisionQueryParams();
            }
            ref = &collisionMap[teamEnum::playerTeam];
            ref->AddIgnoredActor(actor);

            if(collisionMap.find(teamEnum::enemyTeam) == collisionMap.end()){
                collisionMap[teamEnum::enemyTeam] =  FCollisionQueryParams();
            }
            ref = &collisionMap[teamEnum::enemyTeam];
            ref->AddIgnoredActor(actor);
        }*/

        //add to all
        addActorToIgnoredAllParams(actor);
    }
}

/// @brief adds a ignored actor to the all ignore params
/// @param actor actor to ignore
void EntityManager::addActorToIgnoredAllParams(AActor *actor){
    if(actor != nullptr){
        //collisionIgnoreParams.AddIgnoredActor(actor);
    
        if(APathFinder *p = APathFinder::instance()){
            p->addActorToIgnoreRaycastParams(actor);
            
        }
    }
}







/**
 * ---- SPAWN SECTION HERE ----
 */


/// @brief spawns an entity in the world
/// @param world 
/// @param Location 
AEntityScript* EntityManager::spawnEntity(UWorld* world, FVector &Location) {

    AGcLauncher *gc = AGcLauncher::Instance();
    if(gc){
        // gc->Add<E>(uobject*, e);
        // T* gc->Get<T, E>(UClass_T*, e);
        AEntityScript *found = gc->collection.Get<AEntityScript,teamEnum>(
            AEntityScript::StaticClass(), teamEnum::neutralTeam
        );

        //kleiner tessttt, weiss net wo es sonst hin soll wenn es in gc auch gespawned wird
        if(found){
            addActorToIgnoreRaycastParams(found, teamEnum::neutralTeam);
            found->SetActorLocation(Location);
            found->init();
            return found;
        }
    }

    /*
    //get from list if any left
    if(entityList.hasActorsLeft()){
        AEntityScript *entity = entityList.getFirstActor();
        if(entity != nullptr){
            entity->SetActorLocation(Location);
            entity->init();
            return entity;
        }
    }*/

    //else: create
    if(assetManager *a = assetManager::instance()){
        UClass *bp = a->Find<entityEnum, UClass>(entityEnum::entity_enum);
        // a->findBp(entityEnum::entity_enum);
        if(bp != nullptr){
            AActor *actor = spawnAactor(world, bp, Location);
            if(actor != nullptr){
                addActorToIgnoreRaycastParams(actor, teamEnum::neutralTeam);
                AEntityScript *casted = Cast<AEntityScript>(actor);
                if(casted != nullptr){
                    casted->init();
                    return casted;
                }
            }
        }
    }

    return nullptr;
}

/// @brief spawns an human entity in the world
/// @param world 
/// @param Location 
AHumanEntityScript* EntityManager::spawnHumanEntity(UWorld* world, FVector &Location, teamEnum team) {

    AGcLauncher *gc = AGcLauncher::Instance();
    if(gc){
        // gc->Add<E>(uobject*, e);
        // T* gc->Get<T, E>(UClass_T*, e);
        AHumanEntityScript *human = gc->collection.Get<AHumanEntityScript,teamEnum>(
            AHumanEntityScript::StaticClass(), team
        );

        if(human){
            human->SetActorLocation(Location);
            human->init();
            human->setTeam(team);
            addActorToIgnoreRaycastParams(human, team);

            return human;
        }
    }



    /*
    //get from list if any left
    if(humanEntityMap.hasActorsLeft(team)){

        AHumanEntityScript *human = humanEntityMap.getFirstActor(team);
        if(human != nullptr){
            //DebugHelper::showScreenMessage("human from list !", FColor::Yellow);
            human->SetActorLocation(Location);
            human->init();
            

            return human;
        }
    }*/



    //object pool will spawn.
    /*
    if(assetManager *a = assetManager::instance()){
       UClass *bp = a->Find<entityEnum, UClass>(entityEnum::human_enum);
       // a->findBp(entityEnum::human_enum);
       if (bp != nullptr)
       {
           AActor *actor = spawnAactor(world, bp, Location);
           if (actor != nullptr)
           {
               addActorToIgnoreRaycastParams(actor, team);
               AHumanEntityScript *casted = Cast<AHumanEntityScript>(actor);
               if (casted != nullptr)
               {
                   casted->SetActorLocation(Location);
                   casted->init();
                   casted->setTeam(team);
                   return casted;
               }
           }
        }
    }*/
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




    /*
    //weapon map testing
    if(weaponMap.hasActorsLeft(typeToSpawn)){
        Aweapon *fromManager = weaponMap.getFirstActor(typeToSpawn);
        if(fromManager != nullptr){
            fromManager->showItem(true);

            //testing as default sight
            fromManager->applySight(weaponAttachmentEnum::iron_sight);
            return fromManager;
        }
    }*/

    UClass *selectedBp = nullptr;
    if(assetManager *a = assetManager::instance()){
        selectedBp = a->Find<weaponEnum, UClass>(typeToSpawn);
        // a->findBp(typeToSpawn);
    }

    if(selectedBp != nullptr){

        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            // T* gc->Get<T, E>(UClass_T*, e);
            Aweapon *weapon = gc->collection.Get<Aweapon,weaponEnum>(selectedBp, typeToSpawn);

            if(weapon){
                addActorToIgnoreRaycastParams(weapon, teamEnum::neutralTeam);
                weapon->showItem(true);
                //testing as default sight
                weapon->applySight(weaponAttachmentEnum::iron_sight);


                return weapon;
            }
        }




        /*
        AActor *spawned = spawnAactor(world, selectedBp, Location);
        if(spawned != nullptr){
            addActorToIgnoreRaycastParams(spawned, teamEnum::neutralTeam);
        }
        Aweapon *w = Cast<Aweapon>(spawned);
        if(w != nullptr){
            //show weapon
            w->showItem(true);

            //testing as default sight
            w->applySight(weaponAttachmentEnum::iron_sight);

            return w;
        }*/
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

        DebugHelper::logMessage("EntityManager: Try Spawn Throwable");
        DebugHelper::showScreenMessage("EntityManager: Try Spawn Throwable");
        UClass *fromMap = nullptr;
        if(assetManager *a = assetManager::instance()){
            fromMap = a->Find<throwableEnum, UClass>(type);
            // a->findBp(type);

            if(!fromMap){
                DebugHelper::logMessage("EntityManager: throwable asset invalid!");
                DebugHelper::showScreenMessage("EntityManager: throwable asset invalid!");
            }
        }
        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc && fromMap){
            // gc->Add<E>(uobject*, e);
            // T* gc->Get<T, E>(UClass_T*, e);
            AthrowableItem *item = gc->collection.Get<AthrowableItem,throwableEnum>(fromMap, type);
            if(item != nullptr){
                DebugHelper::logMessage("EntityManager: Found thrwable!");
                DebugHelper::showScreenMessage("EntityManager: Found thrwable!");

                addActorToIgnoreRaycastParams(item, teamEnum::neutralTeam);

                item->SetActorLocation(location);
                item->reset();

                return item;
            }
        }


        /*
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
            fromMap = a->Find<throwableEnum, UClass>(type);
            // a->findBp(type);
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
        }*/
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



    UClass *selectedBp = nullptr;
    if(assetManager *a = assetManager::instance()){
        selectedBp = a->Find<weaponEnum, UClass>(weaponEnum::thrower);
        // a->findBp(weaponEnum::thrower);
    }
    AGcLauncher *gc = AGcLauncher::Instance();
    if(gc && selectedBp){

        //thrower weapon is also added under weapon enum and not own subclass.
        //get by base pointer!
        // gc->Add<E>(uobject*, e);
        // T* gc->Get<T, E>(UClass_T*, e); 
        Aweapon *base = gc->collection.Get<Aweapon,weaponEnum>(selectedBp, weaponEnum::thrower);
        if(base != nullptr){
            AthrowerWeapon *casted = Cast<AthrowerWeapon>(base);
            if(casted){
                addActorToIgnoreRaycastParams(casted, teamEnum::neutralTeam);
                
                casted->setThrowableType(typeToSpawn);
                return casted;
            }
        }
    }







    /*
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
            selectedBp = a->Find<weaponEnum, UClass>(weaponEnum::thrower);
            // a->findBp(weaponEnum::thrower);
        }
        AActor *spawned = spawnAactor(world, selectedBp, Location);
        weapon = Cast<AthrowerWeapon>(spawned);
    }

    if(weapon != nullptr){
        weapon->setThrowableType(typeToSpawn);
        return weapon;
    }*/
    return nullptr;
}

/// @brief spawns a custom mesh actor at a given location without any mesh.
/// @param world world to spawn in
/// @param location lcoation to spawn at
/// @return custom mesh actor on success, or nullptr if not
AcustomMeshActor *EntityManager::spawnAcustomMeshActor(UWorld *world, FVector &location){

    UClass *bp = AcustomMeshActor::StaticClass();
    AGcLauncher *gc = AGcLauncher::Instance();
    if(gc && bp){

        // gc->Add<E>(uobject*, e);
        // T* gc->Get<T, E>(UClass_T*, e); 
        AcustomMeshActor *found = gc->collection.Get<AcustomMeshActor>(bp);
        if(found != nullptr){
            found->SetActorLocation(location);
            return found;
        }
    }
    

    
    
    
    
    
    /*
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

    }*/
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
        UMaterialInterface *material = am->Find<materialEnum, UMaterial>(materialType); // am->findMaterial(materialType);
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
    
    
    UClass *bp = nullptr; // getParticleBp(enumtype);

    if(assetManager *am = assetManager::instance()){
        bp = am->Find<particleEnum, UClass>(enumtype); // am->findBp(enumtype);
    }

    
    AGcLauncher *gc = AGcLauncher::Instance();
    if(gc && bp){

        // gc->Add<E>(uobject*, e);
        // T* gc->Get<T, E>(UClass_T_toSpawn*, e); 
        Aparticle *found = gc->collection.Get<Aparticle,particleEnum>(bp,enumtype);
        if(found != nullptr){
            found->SetActorLocation(location);
            found->setParticleType(enumtype); //set the partcle type on start
            found->applyImpulse(dir, speed, lifeTime);
            //return found;
        }
    }

    /*
    if(world != nullptr){


        
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
    }*/
    
}


/// @brief creates an indivudual particle from an material?
/// @param world to spawn in
/// @param UMaterial material to apply
/// @param location to spawn at
/// @param dir direction of impulse
/// @param speed speed to apply
void EntityManager::createParticle(
    UWorld *world, 
    UMaterialInterface *materialToApply,
    FVector &location, 
    FVector &dir, 
    float speed, 
    float lifeTime
){
    
    UClass *bp = nullptr; // getParticleBp(enumtype);

    if(assetManager *am = assetManager::instance()){
        bp = am->Find<particleEnum, UClass>(particleEnum::particleNone_enum);
        // am->findBp(particleEnum::particleNone_enum); //none here.
    }
    
    AGcLauncher *gc = AGcLauncher::Instance();
    if(gc && bp){

        // gc->Add<E>(uobject*, e);
        // T* gc->Get<T, E>(UClass_T_toSpawn*, e); 
        Aparticle *found = gc->collection.Get<Aparticle,particleEnum>(bp,particleEnum::particleNone_enum);
        if(found != nullptr){
            found->SetActorLocation(location);
            found->setParticleType(particleEnum::particleNone_enum); //set the partcle type on start
            
            FVector scale(0.5f, 0.5f, 0.5f);
            found->applyImpulse(dir, speed, lifeTime, materialToApply, scale);
            
            
            //return found;
        }
    }






    /*
    if(world != nullptr){
        UClass *bp = nullptr; // getParticleBp(enumtype);

        if(assetManager *am = assetManager::instance()){
            bp = am->Find<particleEnum, UClass>(particleEnum::particleNone_enum);
            // am->findBp(particleEnum::particleNone_enum); //none here.
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
    }*/
    
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

   
    FVector location(0, 0, 0);

    AcustomMeshActor *customMesh = spawnAcustomMeshActor(world, location);
    if(customMesh != nullptr){

        customMesh->createTwoSidedQuad(
            a, b, c, d,
            materialEnum::wallMaterial
        );

    }

    
}



