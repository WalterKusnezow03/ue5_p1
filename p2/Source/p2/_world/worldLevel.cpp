// Fill out your copyright notice in the Description page of Project Settings.


#include "worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/rooms/layoutCreator/layoutMaker.h"
#include "p2/util/TVector.h"
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


/// IS RESET FROM GAME MODE SUBCLASS, MUST BE CALLED ON END PLAY!
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

    bool debugCreate = true; //dont create terrain for debugging 
    //disabled for debugging
    if(debugCreate){
        if (!isTerrainInited && world != nullptr){
            createTerrain(world, 100); // 100m
        }
    }

    //create rooms
    DebugCreateRooms(world);

    //edge collector must be added here later
    createPathFinder(world);

    humanBotsOnStart(world);

    //testing
    DebugHelper::Debugtest(world);

    //debugBezier(world);
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

        terrainPointer->debugCreateTerrain(world); //new test
        return;

        //create terrain
        EntityManager *e = entityManager();
        if(e != nullptr){

            terrainPointer->createTerrain(world, meters);

            //request mesh actors and apply terrain
            bool applyAndCreateterrain = true;
            if(applyAndCreateterrain){
                int numberCreated = terrainPointer->chunkNum();
                std::vector<AcustomMeshActor *> meshes = e->requestMeshActors(world, numberCreated);
                



                terrainPointer->applyTerrainDataToMeshActors(meshes);
            }
            
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





void worldLevel::debugBezier(UWorld *world){
    if(world == nullptr){
        return;
    }

    std::vector<FVector2D> anchors;
    TVector<FVector2D> outputCurve;
    float oneMeter = 100;

    anchors.push_back(FVector2D(0, 300));
    anchors.push_back(FVector2D(500, 100));
    anchors.push_back(FVector2D(1000, 200));
    anchors.push_back(FVector2D(1500, 100));
    anchors.push_back(FVector2D(2000, 200));

    bezierCurve b;
    b.calculatecurve(
		anchors,
		outputCurve,
		oneMeter
	);

    std::vector<FVector> d3Vec;
    for (int i = 0; i < outputCurve.size(); i++){
        FVector2D copy = outputCurve[i];
        FVector projected(copy.X, 0, copy.Y);
        d3Vec.push_back(projected);
    }

    DebugHelper::showLine(world, d3Vec, FColor::Black);

    for (int i = 0; i < d3Vec.size(); i++){
        FVector current = d3Vec.at(i);
        FVector offset = current + FVector(0, 0, 100);
        DebugHelper::showLineBetween(world, current, offset, FColor::Black);
    }
}