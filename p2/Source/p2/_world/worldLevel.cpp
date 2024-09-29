// Fill out your copyright notice in the Description page of Project Settings.


#include "worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/rooms/RoomManager.h"
#include "p2/meshgen/terrainCreator.h"

worldLevel::worldLevel()
{
    
}

worldLevel::~worldLevel()
{
    
}

//static vars init:
bool worldLevel::isTerrainInited = false;
EntityManager *worldLevel::entityManagerPointer = nullptr;
OutpostManager *worldLevel::outpostManagerPointer = nullptr;
terrainCreator *worldLevel::terrainPointer = nullptr;
RoomManager *worldLevel::roomManagerPointer = nullptr;

/// @brief clears all pointers -> call only on very begin or very end of level!
/// -> entity manager: holds all entities and room, terrain basic assets!
/// -> outpostManager: holds all outposts and assigns them on request
/// -> terrainPointer: creates the terrain once and holds the whole mesh for faster ground detection
/// -> pathfinder singleton instance -> all nodes will be wiped
void worldLevel::resetWorld(UWorld *world){
    if(entityManagerPointer != nullptr){
        delete entityManagerPointer;
        entityManagerPointer = nullptr;
    }
    if(outpostManagerPointer != nullptr){
        delete outpostManagerPointer;
        outpostManagerPointer = nullptr;
    }
    if(terrainPointer != nullptr){
        delete terrainPointer;
        terrainPointer = nullptr;
        isTerrainInited = false;
    }

    //clears all nodes from graph
    if(PathFinder *p = PathFinder::instance(world)){
        p->clear();
    }
}

/// @brief will init the terrain, keep in mind that all assets must be loaded before!
/// @param world 
void worldLevel::initWorld(UWorld *world){

    bool create = true;
    //disabled for debugging
    if(create){
        if (!isTerrainInited && world != nullptr){
            createTerrain(world, 300); // 100
        }
    }
    
    //edge collector must be added here later
    createPathFinder(world);




    //testing
    DebugHelper::Debugtest(world);
}

/**
 * ATTENTION: PathFinder Collect edges will only be called from this class and only once on level start
 */
void worldLevel::createPathFinder(UWorld *WorldIn){
    if(WorldIn == nullptr){
        return;
    }

    if (WorldIn)
    {
        EdgeCollector c = EdgeCollector();
        c.getAllEdges(WorldIn); //pushes them to the navmesh on its own
    }


}



/**
 * 
 * INSTANCE POINTERS
 * 
 */



/// @brief intended for one time use only, do not delete, do not save
/// @return returns pointer to entitymanager
EntityManager *worldLevel::entityManager(){
    if(entityManagerPointer == nullptr){
        entityManagerPointer = new EntityManager();
    }

    return entityManagerPointer;
}

/// @brief returns the outpost manager to ask for the nearest outpost
/// @return will return the pointer
OutpostManager * worldLevel::outpostManager(){
    if(outpostManagerPointer == nullptr){
        outpostManagerPointer = new OutpostManager();
    }
    return outpostManagerPointer;
}

/// @brief returns the pointer to the room manager, save assets inside the proiveded object by the pointer
/// DO NOT DELETE
/// @return returns the pointer to the room manager, use to save room prefabs and instantiate buildings
RoomManager *worldLevel::roomManager(){
    if(roomManagerPointer == nullptr){
        roomManagerPointer = new RoomManager();
    }
    return roomManagerPointer;
}

/**
 * 
 * 
 * TERRAIN
 * 
 * 
 */



/// @brief creates the terrain if not yet created
/// @param world world to spawn in
/// @param meters meters of the terrain targeted
void worldLevel::createTerrain(UWorld *world, int meters){
    if(isTerrainInited){
        return;
    }
    DebugHelper::showScreenMessage("try init terrain", FColor::Purple);

    if(world != nullptr){
        //create new terrain pointer if needed
        if(terrainPointer == nullptr){
            terrainPointer = new terrainCreator();
        }

        //create terrain
        EntityManager *e = entityManager();
        if(e != nullptr){
            terrainPointer->createTerrain(world, meters);

            //choose some size between 0 and meters for pos and some scale (lets say 15 - 30?) 
            //some how block the are which is filled?
            int terrainSizehalf = (meters * terrainCreator::ONEMETER) / 2;
            int roomsizeMeter = 15;
            int roomsizeWorldScale = roomsizeMeter * 100;
            FVector locationToSpawn(
                FVectorUtil::randomNumber(0, terrainSizehalf - roomsizeWorldScale),
                FVectorUtil::randomNumber(0, terrainSizehalf - roomsizeWorldScale),
                0
            );
            int spawnheight = terrainPointer->getHeightFor(locationToSpawn);
            locationToSpawn.Z = spawnheight;

            //create room layouts to embed

            //flatten terrain / override height
            terrainPointer->setFlatArea(locationToSpawn, roomsizeMeter, roomsizeMeter);

            //create rooms
            if(RoomManager *r = roomManager()){
                r->createABuilding(world, locationToSpawn, roomsizeMeter, roomsizeMeter);
            }

            //request mesh actors and apply terrain
            int numberCreated = terrainPointer->chunkNum();
            std::vector<AcustomMeshActor *> meshes = e->requestMeshActors(world, numberCreated);

            terrainPointer->applyTerrainDataToMeshActors(meshes);

            //finally set created to true
            isTerrainInited = true;
        }
    }
}


int worldLevel::getGroundHeight(FVector &pos){
    if(terrainPointer != nullptr){
        return terrainPointer->getHeightFor(pos);
    }
    return pos.Z;
}