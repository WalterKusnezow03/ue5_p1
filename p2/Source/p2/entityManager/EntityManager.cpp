// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManager.h"
#include "GameCore/EntityGC/EntityManagerGeneric.h"
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
#include "DebugPlugin/DebugHelper.h"
#include "PathFinder/pathFinding/PathFinder.h"
#include "p2/weapon/throwerWeapon.h"

#include "GcGameCore/Launcher/GcLauncher.h"

#include "IkHumanoidModell/SharedRaycastParams/SharedRaycastParamManager.h"
#include "p2/ui/3Dui/GamePlayWidgets/InteractWidget/InteractWidgetActor.h"

#include "p2/ui/3Dui/GamePlayWidgets/Enum/EWorldDynamicWidgetEnum.h"

#include "p2/vfx/base/VFXActor.h"


#include <map>


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
        
    }
}

/// @brief adds a weapon to the entity manager, and hides it
/// @param weaponIn 
void EntityManager::add(Aweapon *weaponIn){
    
    if(weaponIn != nullptr){
        FVector hiddenLocation = FVector(0, 0, -10000);
        weaponIn->SetActorLocation(hiddenLocation);
        weaponIn->showWeapon(false);
        
        
        
        
        weaponEnum type = weaponIn->weaponType();
        
        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add<weaponEnum>(weaponIn, type);
        }

        
    }
}

/// @brief adds a throwable to the entity manager
/// @param throwableItem 
void EntityManager::add(AthrowableItem *throwableItem){
    if(throwableItem != nullptr){
        throwableEnum type = throwableItem->getType();

        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add<throwableEnum>(throwableItem, type);
        }
    }
}



/// @brief adds a mesh actor the entity manager
/// @param meshActorIn 
void EntityManager::add(AcustomMeshActor *meshActorIn){
    if(meshActorIn != nullptr){

        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add(meshActorIn);
        }
    }
}



void EntityManager::add(AWorldDynamicWidgetActor *actorIn){
    if(actorIn){
        EWorldDynamicWidgetEnum typeWidget = actorIn->GetWidgetType();
        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add<EWorldDynamicWidgetEnum>(actorIn, typeWidget);
        }
    }
}


#include "p2/vfx/type/EVFXActorType.h"
void EntityManager::add(AVFXActor *actorIn){
    if(actorIn){
        EVFXActorType typeAdded = actorIn->typeVFX;
        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            // gc->Add<E>(uobject*, e);
            gc->collection.Add<EVFXActorType>(actorIn, typeAdded);
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
 * ---- SPAWN / OBJECT POOL SECTION HERE ----
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
            
            found->init(Location);
            return found;
        }
    }

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
            
            human->init(Location);
            human->setTeam(team);
            addActorToIgnoreRaycastParams(human, team);

            return human;
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

                weapon->ResetFlagsAndProperties();

                return weapon;
            }
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




AWorldDynamicWidgetActor *EntityManager::spawnAWorldDynamicWidgetActor(
    EWorldDynamicWidgetEnum typeWidgetActorToSpawn,
    UWorld *world, 
    USceneComponent *attachTo,
    FVector relativeLocation
){
    if(world && attachTo){
        UClass *selectedBp = nullptr;
        if(assetManager *a = assetManager::instance()){
            selectedBp = a->Find<EWorldDynamicWidgetEnum, UClass>(typeWidgetActorToSpawn);
        }

        if(selectedBp != nullptr){

            AGcLauncher *gc = AGcLauncher::Instance();
            if(gc){
                // gc->Add<E>(uobject*, e);
                // T* gc->Get<T, E>(UClass_T*, e);

                //will spawn if not found by blueprint.
                AWorldDynamicWidgetActor *widgetActor = 
                    gc->collection.Get<AWorldDynamicWidgetActor,EWorldDynamicWidgetEnum>(
                        selectedBp, typeWidgetActorToSpawn
                    );

                if(widgetActor){
                    widgetActor->InitFromObjectPool(attachTo, relativeLocation);

                    return widgetActor;
                }
            }

        }
    }
    return nullptr;
}



    




















/// ---> to be refactured, very old code.
/** 
* ---- PARTICLE / EXPLOSION SECTION ----
*/


/// @brief creates an visual explosion at a given location
/// @param world 
/// @param location 
void EntityManager::createExplosion(FVector &location){
    DebugHelper::showScreenMessage("EntityManager::createExplosion -> A", FColor::Orange);
    if(AVFXActor *actor = spawnAVFXActor(EVFXActorType::EExplosion)){
        actor->SetActorLocation(location);
        DebugHelper::showScreenMessage("EntityManager::createExplosion -> B", FColor::Green);
    }
}

AVFXActor *EntityManager::spawnAVFXActor(EVFXActorType typeVFX){
    
    UClass *selectedBp = nullptr;
    if(assetManager *a = assetManager::instance()){
        selectedBp = a->Find<EVFXActorType, UClass>(typeVFX);
    }
    if(selectedBp != nullptr){
        AGcLauncher *gc = AGcLauncher::Instance();
        if(gc){
            //will spawn if not found by blueprint.
            AVFXActor *actor = gc->collection.Get<AVFXActor,EVFXActorType>(
                selectedBp, typeVFX
            );

            if(actor){
                actor->Init();
                return actor;
            }
        }
    }
    return nullptr;
}
