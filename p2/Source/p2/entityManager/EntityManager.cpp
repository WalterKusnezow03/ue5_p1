// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityManager.h"
#include "EntityManagerGeneric.h"
#include "p2/particleSystem/particle.h"
#include "p2/particleSystem/particleEnum.h"
#include "p2/meshgen/terrainCreator.h"
#include "p2/meshgen/customMeshActor.h"


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
	weaponBpClass = nullptr;
    pistolBpClass = nullptr;

    
    grenadeBpClass = nullptr;
    rocketBpClass = nullptr;
    molotovBpClass = nullptr;
    defaultThrower = nullptr; //immer am anfang setzen, sonst crash potenziell!
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
    defaultThrower = nullptr;
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

        //add team checkup here, remove from all alive map
        teamEnum type = humanEntity->getTeam();
        ALIVE_HumanEntitiesMap.erase(type, humanEntity);//erase entity if found
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

            //team will be just enemies for now
            teamEnum t = teamEnum::enemyTeam;
            human->setTeam(t);
            ALIVE_HumanEntitiesMap.add(t, human);

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


    //default spawn if needed
    UClass *selectedBp = weaponBpClass; //to created, default is stick gun

    std::map<weaponEnum, UClass *> map;
    map[weaponEnum::assaultRifle] = assaultRifleBpClass;
    map[weaponEnum::pistol] = pistolBpClass;

    selectedBp = map[typeToSpawn];
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



//testing thrower weapons (works for grenades)
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
        AActor *spawned = spawnAactor(world, defaultThrower, Location);
        weapon = Cast<AthrowerWeapon>(spawned);
    }

    if(weapon != nullptr){
        weapon->setThrowableType(typeToSpawn);
        return weapon;
    }
    return nullptr;
    /*
    //old
    if(defaultThrower != nullptr){
        
        AActor *spawned = spawnAactor(world, defaultThrower, Location);
        AthrowerWeapon *w = Cast<AthrowerWeapon>(spawned);
        if(w != nullptr){
            w->setThrowableType(typeToSpawn);
            return w;
        }
    }
    return nullptr;
    */
}

//default "thrower / werfer"
void EntityManager::setDefaultThrowerClassBp(UClass *uIn){
    if(defaultThrower == nullptr){
        defaultThrower = uIn;
    }
}



/** 
* ---- PARTICLE / EXPLOSION SECTION ----
*/


/// @brief creates an explosion at a given location
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
        UClass *bp = getParticleBp(enumtype);

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


/// @brief sets all particle blueprints
/// @param uIn 
/// @param type 
void EntityManager::setparticleBp(UClass *uIn, particleEnum type){

    if(uIn != nullptr){
        switch(type){
        case particleEnum::smoke_enum:
            smokeParticleBp = uIn;
            break;
        case particleEnum::fire_enum:
            fireParticleBp = uIn;
            break;
        }
    }

}

UClass *EntityManager::getParticleBp(particleEnum type){
    switch(type){
    case particleEnum::smoke_enum:
        return smokeParticleBp;
    case particleEnum::fire_enum:
        return fireParticleBp;
    }
    return nullptr;
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

/// @brief adds a door to the appropiate room manager, types must be made up later!
/// @param uclassIn 
void EntityManager::setDooruClassBp(UClass *uclassIn){
    if(uclassIn != nullptr){
        roomType1Manager.addDoor(uclassIn);
    }else{
        DebugHelper::showScreenMessage("door uclass was nullptr! - entity manager", FColor::Red);
    }
}

void EntityManager::setWindowuClassBp(UClass *uclassIn){
    if(uclassIn != nullptr){
        roomType1Manager.addWindow(uclassIn);
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
void EntityManager::setEmptyMeshUClassBp(UClass *uclassIn){
    if(uclassIn != nullptr){
        emptyCustomMeshActorBp = uclassIn;
    }
}


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


/**
 * ---- section for materials -----
 */

/// @brief add a material to be used on any mesh by material enum type
/// @param type 
/// @param material 
void EntityManager::addMaterial(materialEnum type, UMaterial *material){
    if(material != nullptr){
        materialMap[type] = material;
    }
}

/// @brief get any material by enum type
/// @param type type to find
/// @return material or nullptr if none found
UMaterial *EntityManager::getMaterial(materialEnum type){
    if(materialMap.find(type) != materialMap.end()){
        return materialMap[type];
    }
    return nullptr;
}




/**
 * ---- SECTION FOR TARGET GETTING ----
 */

/// @brief WILL NOT BE USED YET! UNCLEAR IF USED AT ALL! -- would be called on each frame
/// or in some other intervall like each second for example
/// @param ownActor 
/// @param ownTeam 
/// @return 
AHumanEntityScript *EntityManager::getNearestTarget(AHumanEntityScript *ownActor, teamEnum ownTeam){
    if(ownActor != nullptr){
        if(ALIVE_MAP.find(ownTeam) != ALIVE_MAP.end()){
            std::vector<AHumanEntityScript *> &ref = ALIVE_MAP[ownTeam];

            FVector a = ownActor->GetActorLocation();

            //find closest by quadratic distance
            int quadDistance = FVectorUtil::intInfinity();
            AHumanEntityScript *close = nullptr;
            for (int i = 0; i < ref.size(); i++)
            {
                AHumanEntityScript *h = ref.at(i);
                if (h != nullptr)
                {
                    if(h->isActivatedForUpdate()){
                        FVector b = h->GetActorLocation();
                        int newdist = FVectorUtil::quadraticDist(a, b);
                        if(newdist < quadDistance){
                            quadDistance = newdist;
                            close = h;
                        }
                    }
                }
            }
            return close;
        }
    }
    return nullptr;
}