// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManager.h"
#include "EntityManagerGeneric.h"
#include "p2/particleSystem/particle.h"
#include "p2/particleSystem/particleEnum.h"
#include "p2/meshgen/terrainCreator.h"
#include "p2/meshgen/customMeshActor.h"
#include "p2/gamestart/assetManager.h"

#include "Engine/World.h"
#include "p2/entities/EntityScript.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "p2/throwableItems/throwableEnum.h"
#include "p2/throwableItems/throwableItem.h"
#include "p2/util/FVectorUtil.h"
#include "p2/rooms/room.h"

#include <map>


//set static ref to nullptr
EntityManager *EntityManager::instancePointer = nullptr;

/// @brief if you receive this pointer you are NOT ALLOWED to delete this instance!
/// @return entityManager instance pointer 
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
	
}

EntityManager::~EntityManager()
{
    entityBpClass = nullptr;
	humanEntityBpClass = nullptr;
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
        weaponIn->showWeapon(false);

        weaponEnum type = weaponIn->readType();

        FVector hiddenLocation = FVector(0, 0, -1000);
        weaponIn->SetActorLocation(hiddenLocation);

        //new map manager
        weaponMap.add(type, weaponIn);

    }
}

void EntityManager::add(AthrowableItem *throwableItem){
    if(throwableItem != nullptr){
        throwableEnum type = throwableItem->getType();
        throwableMap.add(type, throwableItem);

    }
}



void EntityManager::add(Aparticle *particleIn){
    if(particleIn != nullptr){
        particleEnum type = particleIn->getType();
        particleMap.add(type, particleIn);
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
AHumanEntityScript* EntityManager::spawnHumanEntity(UWorld* world, FVector &Location) {

    //get from list if any left
    if(humanEntityList.hasActorsLeft()){
        AHumanEntityScript *human = humanEntityList.getFirstActor();
        if(human != nullptr){
            //DebugHelper::showScreenMessage("human from list !", FColor::Yellow);

            human->init();
            human->SetActorLocation(Location);

            return human;
        }
    }

    

    //else create new one
    AActor *actor = spawnAactor(world, humanEntityBpClass, Location);
    AHumanEntityScript *casted = Cast<AHumanEntityScript>(actor);
    if(casted != nullptr){
        casted->init();
        //DebugHelper::showScreenMessage("try spawn human");
        return casted;
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

    /*
    //default spawn if needed
    UClass *selectedBp = weaponBpClass; //to created, default is stick gun

    std::map<weaponEnum, UClass *> map;
    map[weaponEnum::assaultRifle] = assaultRifleBpClass;
    map[weaponEnum::pistol] = pistolBpClass;

    selectedBp = map[typeToSpawn];
    */
    UClass *selectedBp = nullptr;
    if(assetManager *a = assetManager::instance()){
        selectedBp = a->findBp(typeToSpawn);
    }

    if(selectedBp != nullptr){
        AActor *spawned = spawnAactor(world, selectedBp, Location);
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
        if(emptyCustomMeshActorBp != nullptr){
            AActor *actor = spawnAactor(world, emptyCustomMeshActorBp, location);
            if(actor != nullptr){

                AcustomMeshActor *customMesh = Cast<AcustomMeshActor>(actor);
                if(customMesh != nullptr){
                    return customMesh;
                }
            }
        }
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

            createParticle(world, particleEnum::smoke_enum, location, dir, speed, smokeLifeTime);


            //fire testing
            if(i < 10){
                createParticle(world, particleEnum::fire_enum, location, dir, speed * 10, fireLifeTime);
            }
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






/**
 * 
 * 
 * ---- REFERENCE SETTINGS / SET U CLASS SECTION ----
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






/** 
 * 
 * ----- SECTION FOR ROOM CREATION -----
 * 
*/


/// @brief adds a room to the correct room manager
/// room types must be added later
/// @param world 
/// @param uclass 
void EntityManager::setRoomuClassBp(UWorld *world, UClass *uclass){
    if(uclass != nullptr && world != nullptr){
        roomType1Manager.add(world, uclass);
    }
}







/// @brief creates a room layout and spawns all rooms for it
/// different room types will be added later
/// @param worldIn world to spawn in
/// @param location 
/// @param xscale 
/// @param yscale 
void EntityManager::createALayout(UWorld *worldIn, FVector &location, int xscale, int yscale){
    
    DebugHelper::showScreenMessage("TRY CREATE ROOMS", FColor::Red);

    if(worldIn != nullptr){
        //must be selected a room type too
        roomType1Manager.createALayout(worldIn, location, xscale, yscale);
    }

}






/**
 * ---- SECTION FOR TERRAIN ----
 * 
 */

/// @brief sets the emoty mesh actor to spawn an process meshes, nesecarry for terrain and other meshes
/// @param uclassIn uclass to spawn
void EntityManager::setEmptyMeshUClassBp(UClass *uclassIn){
    if(uclassIn != nullptr){
        emptyCustomMeshActorBp = uclassIn;
    }
}

/// @brief creates an terrain from chunk size (10meters each)
/// @param worldIn world to spawn in 
/// @param chunks chunks (in both x and y direction to create)
void EntityManager::createTerrain(UWorld *worldIn, int chunks){
    if(worldIn != nullptr){
        terrainCreator c;
        c.createterrain(worldIn, chunks);

        //return; //debugging

        //created chunks in rerquested size, let terrain creator process and populate them
        int requestedCount = c.chunkNum();
        std::vector<AcustomMeshActor *> requestedActors;

        //for each chunk data: spawn new chunk
        if(emptyCustomMeshActorBp != nullptr){
            for (int i = 0; i < requestedCount; i++){
                //read location from chunk data
                FVector location(0, 0, 0);

                AActor *actor = spawnAactor(worldIn, emptyCustomMeshActorBp, location);
                if(actor != nullptr){
                    AcustomMeshActor *casted = Cast<AcustomMeshActor>(actor);
                    if(casted != nullptr){
                        
                        //apply mesh data for the chunk
                        requestedActors.push_back(casted);
                    }
                }
            }
        }

        c.applyTerrainDataToMeshActors(requestedActors);
        requestedActors.clear();

    }
}

/// @brief creates a mesh from the given vertecies
/// @param world world to spawn in
/// @param vertecies vertecies
void EntityManager::createAMesh(UWorld *world, std::vector<std::vector<FVector>> &vertecies){
    FVector location(0, 0, 0);
    AActor *actor = spawnAactor(world, emptyCustomMeshActorBp, location);
    if(actor != nullptr){

        AcustomMeshActor *customMesh = Cast<AcustomMeshActor>(actor);
        if(customMesh != nullptr){
            customMesh->process2DMap(vertecies);
        }
    }
}



void EntityManager::createTwoSidedQuad(UWorld *world, FVector &a, FVector &b, FVector &c, FVector &d){
    //implementation needs to be tested!

    if(assetManager *am = assetManager::instance()){
        FVector location(0, 0, 0);

        AActor *actor = spawnAactor(world, emptyCustomMeshActorBp, location);
        if(actor != nullptr){

            AcustomMeshActor *customMesh = Cast<AcustomMeshActor>(actor);
            if(customMesh != nullptr){

                customMesh->createTwoSidedQuad(
                    a, b, c, d, am->findMaterial(materialEnum::wallMaterial)
                );
            }
        }
    }
}



