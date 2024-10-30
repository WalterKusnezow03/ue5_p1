// Fill out your copyright notice in the Description page of Project Settings.


#include "worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/rooms/layoutCreator/layoutMaker.h"
#include "p2/meshgen/generation/terrainCreator.h"

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


/// @brief clears all pointers -> call only on very begin or very end of level!
/// -> entity manager: holds all entities and room, terrain basic assets!
/// -> outpostManager: holds all outposts and assigns them on request
/// -> terrainPointer: creates the terrain once and holds the whole mesh for faster ground detection
/// -> pathfinder singleton instance -> all nodes will be wiped
void worldLevel::resetWorld(){
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
    PathFinder *p = PathFinder::instance();
    if (p != nullptr)
    {
        //p->clear();

        p->deleteInstance();

        FString s = FString::Printf(TEXT("debug end play: deleted path finder"));
        DebugHelper::logMessage(s);
    }
}

/// @brief will init the terrain, keep in mind that all assets must be loaded before!
/// @param world 
void worldLevel::initWorld(UWorld *world){

    bool debugCreate = false; //dont create terrain for debugging 
    //disabled for debugging
    if(debugCreate){
        if (!isTerrainInited && world != nullptr){
            createTerrain(world, 100); // 100
        }
    }

    //create rooms
    DebugCreateRooms(world);

    //edge collector must be added here later
    createPathFinder(world);

    humanBotsOnStart(world);

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

            /*
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
            }*/

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


/** 
 * 
 * DEBUG HUMAN ENTITIES
 * 
*/
void worldLevel::humanBotsOnStart(UWorld *worldIn){
    if(worldIn == nullptr){
        return;
    }

    EntityManager *e = entityManager();
    if (e != nullptr)
    {
        FVector spawnLocation(1000, 1000, 20);
        spawnLocation.Y += 1000;
        spawnLocation.X += 1000;
        e->spawnHumanEntity(worldIn, spawnLocation, teamEnum::enemyTeam);
    }
}






//debug method create rooms near world origin
void worldLevel::DebugCreateRooms(UWorld *world){
    if(world == nullptr){
        return;
    }


    int roomsizeMeter = 20;
    FVector locationToSpawn(
        roomsizeMeter * -150,
        roomsizeMeter * -150,
        2
    );



    //new layout creator testing
    std::vector<TTouple<int, int>> sizesP;
    sizesP.push_back(TTouple<int, int>(6, 6));
    sizesP.push_back(TTouple<int, int>(10, 10));
    sizesP.push_back(TTouple<int, int>(8, 8));

    std::vector<roomBoundData> outputRooms;

    layoutMaker l;
    l.makeLayout(20, 20, sizesP, outputRooms);
    AroomProcedural::spawnRooms(world, locationToSpawn, outputRooms);
}