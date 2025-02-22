// Fill out your copyright notice in the Description page of Project Settings.


#include "worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/rooms/layoutCreator/layoutMaker.h"
#include "p2/util/TVector.h"
#include "p2/meshgen/customMeshActorWater.h"
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

bool worldLevel::areBotsInited = false;

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
            int meters = 100;
            meters = 1000;
            createTerrain(world, meters); // 100m
        }
    }

    //create rooms
    DebugCreateRooms(world);

    //edge collector must be added here later
    createPathFinder(world);

    //creates one bot, BUT 5 humans will spawn if one outpost is created!
    humanBotsOnStart(world, 1);
        

    //testing
    DebugHelper::Debugtest(world);

    //debugBezier(world);
    debugAngleFinder(world);

    debugCreateWater(world);
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
void worldLevel::humanBotsOnStart(UWorld *worldIn, int count){
    if(worldIn == nullptr){
        return;
    }
    if(worldLevel::areBotsInited){
        return;
    }
    worldLevel::areBotsInited = true;

    EntityManager *e = entityManager();
    if (e != nullptr)
    {
        for (int i = 0; i < count; i++){
            FVector spawnLocation(-1000, -1000, 20);
            spawnLocation.Y += 1000;
            spawnLocation.X += 1000;
            e->spawnHumanEntity(worldIn, spawnLocation, teamEnum::enemyTeam);
        }

        
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




void worldLevel::debugAngleFinder(UWorld *world){

    //return;

    /**
     * achtung infinite loop!
     */

    std::vector<float> bones;
    float lenghtAll = 1000.0f;
    int pieces = 10;
    float part = 200.0f; //mehr als reinpassen
    for (int i = 0; i < pieces; i++){
        bones.push_back(part);
    }

    std::vector<FVector2D> output = worldLevel::findAngles(lenghtAll, bones);

    //draw
    float maxHeight = 0.0f;
    for (int i = 0; i < output.size(); i++){
        FVector2D &current = output.at(i);
        if(current.Y > maxHeight){
            maxHeight = current.Y;
        }
    }

    FVector prev(-1 * 2000.0f, 0, 100.0f);
    float time = 100.0f;
    for (int i = 0; i < output.size(); i++)
    {
        FVector2D current2D = output.at(i);
        FVector current(current2D.X, 0.0f, current2D.Y);
        current += prev;

        FColor color = i % 2 == 0 ? FColor::Red : FColor::Cyan;
        DebugHelper::showLineBetween(world, current, prev, color, time);
        prev = current;
    }
}

std::vector<FVector2D> worldLevel::findAngles(float lengthAll, std::vector<float> &bones){
    std::vector<FVector2D> vec;
    FVector2D axis(lengthAll, 0.0f);

    float sum = 0.0f;
    for (int i = 0; i < bones.size(); i++){
        vec.push_back(FVector2D(bones.at(i), 0.0f));
        sum += bones.at(i);
    }

    FString showString = FString::Printf(TEXT("AngleHelper distance now: %.2f , distance target: %.2f"), sum, lengthAll);
    DebugHelper::logMessage(showString);

    int currentIndex = 0;
    int max = 100000000;
    int i = 0;

    float angleLimit = 30.0f; //test
    float angleStep = 10.0f;
    float angleSoFar = 0.0f;
    while (i < max)
    {
        i++;
        if(i > max){
            break;
        }
        if(sum <= lengthAll){ //reached
            DebugHelper::logMessage("AngleHelperReached");
            break;
        }
        if(currentIndex >= vec.size()){
            //reached by mistake
            if(sum > lengthAll){
                currentIndex = 0;
                angleSoFar = 0.0f;

                //limit update
                angleLimit -= 5.0f;
                if(angleLimit < 0.0f){
                    angleLimit = 0.0f;
                }
            }
            else
            {
                break;
            }
        }

        FVector2D &currentVector = vec.at(currentIndex);
        if(currentVector.X > 0.0f){
            //vorherigen winkel ausgleichen


            //abziehen, drehen, drauf rechnen
            sum -= currentVector.X;
            float rotateAt = angleStep;
            MMatrix::rotateVectorDeg2D(rotateAt, currentVector);
            sum += currentVector.X;

            angleSoFar += rotateAt * 2.0f;
        }
        
        if(currentVector.X <= 0.0f || std::abs(angleSoFar) > angleLimit){
            currentIndex++;
            if(angleSoFar > 0.0f){
                angleStep = std::abs(angleStep) * -1.0f;
            }else{
                angleStep = std::abs(angleStep);
            }
        }
    }
    return vec;
}













void worldLevel::debugCreateWater(UWorld *world){
    if(world != nullptr){

        int sizeX = 100000;
        int sizeY = 100000;
        int detail = 1000;

        EntityManager *pointer = worldLevel::entityManager();
        if(pointer != nullptr){
            AcustomMeshActorWater *actor = pointer->createWater(world, sizeX, sizeY, detail);
        }
    }
}