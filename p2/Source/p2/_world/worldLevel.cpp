// Fill out your copyright notice in the Description page of Project Settings.


#include "worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/rooms/layoutCreator/layoutMaker.h"
#include "p2/util/TVector.h"
#include "p2/meshgen/generation/terrainCreator.h"
#include "p2/meshgen/foliage/MatrixTree.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/meshgen/foliage/ETreeType.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "p2/pathFinding/EdgeCollector.h"
#include "p2/pathFinding/PathFinder.h"
#include "p2/meshgen/specialMeshactors/wingsuitMeshActor.h"
#include "p2/meshgen/foliage/rocks/RockCreator.h"
#include "p2/meshgen/water/customWaterActor.h"
#include "p2/rooms/testing/roomProcedural.h"
#include "p2/DebugHelper.h"
#include "p2/entityManager/referenceManager.h"
#include "CoreMinimal.h"

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
        
    createGroundPane(world);

    //testing
    DebugHelper::Debugtest(world);

    //debugBezier(world);
    debugAngleFinder(world);

    debugCreateWater(world);

    debugCreateRock(world);

    debugCreateWingsuitMesh(world);

    debugMatrix();
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
        isTerrainInited = true;
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
 * 
 * ----- PLAYER TICK TERRAIN ASYNC CREATION & PATHFINDER TASKS------
 * 
 * 
 */
void worldLevel::Tick(float DeltaTime){
    referenceManager *referenceManagerPointer = referenceManager::instance();
    if(referenceManagerPointer != nullptr){
        if(terrainPointer != nullptr){
            FVector playerLocationCopy = referenceManagerPointer->playerLocation();
            terrainPointer->Tick(playerLocationCopy);
        }
    }

    //tick sync tick tasks from pathfinder
    PathFinder *p = PathFinder::instance();
    if(p != nullptr){
        p->Tick();
    }
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

    AroomProcedural::generate(world, roomsizeMeter, roomsizeMeter, locationToSpawn);
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





void worldLevel::createGroundPane(UWorld *world){
    if(world != nullptr){

        int onemeter = 100;
        int size = onemeter * 100;
        FVector location(0, 0, 100);
        std::vector<FVector> verteciesPane = MeshData::create2DQuadVertecies(size, size);
        if(verteciesPane.size() == 4){
            MeshData ground;
            ground.append(verteciesPane[0], verteciesPane[1], verteciesPane[2], verteciesPane[3]);
            ground.calculateNormals();


            FRotator rotation;
            FActorSpawnParameters params;
            AcustomMeshActorBase *SpawnedActor = world->SpawnActor<AcustomMeshActorBase>(
                AcustomMeshActorBase::StaticClass(),
                location,
                FRotator::ZeroRotator,
                params
            );
            if(SpawnedActor != nullptr){
                SpawnedActor->replaceMeshData(ground, materialEnum::stoneMaterial);
                SpawnedActor->ReloadMeshAndApplyAllMaterials();
            }
        }
    }
}


void worldLevel::debugCreateWater(UWorld *world){
    return; //DEBUG

    if(world != nullptr){

        int scaleMeters = 50 * 100;
        AcustomWaterActor::createWaterPane(
            world,
            FVector(-1000,-1000,0),
            scaleMeters
        );
    }
}




void worldLevel::debugCreateRock(UWorld *world){
    if(world != nullptr){
        EntityManager *pointer = worldLevel::entityManager();
        if(pointer != nullptr){

            RockCreator rock;
            MeshData meshData = rock.createMesh();

            meshData = FVectorShape::createSphere(150, 50, true); //NEW DEBUG, fac outside

            FVector location(-2000, 0, 100);
            AcustomMeshActor *actor = pointer->spawnAcustomMeshActor(world, location);

            if(actor != nullptr){
                actor->replaceMeshData(meshData, materialEnum::stoneMaterial);
                actor->ReloadMeshAndApplyAllMaterials();

                actor->enableDebug();
            }
        }
    }
}



void worldLevel::debugCreateWingsuitMesh(UWorld *world){
    if(world != nullptr){
        FVector location(0, 0, 0);
        FRotator rotation;
        FActorSpawnParameters params;
        AwingsuitMeshActor *SpawnedActor = world->SpawnActor<AwingsuitMeshActor>(
            AwingsuitMeshActor::StaticClass(),
            location,
            FRotator::ZeroRotator,
            params
        );
        if(SpawnedActor != nullptr){
            int detail = 20;
            SpawnedActor->initWingsuitMesh(detail);
        }
    }
}





void worldLevel::debugMatrix(){

    std::vector<FVector> checkup = {
        FVector(1, 1, 1),
        FVector(2, 1, 1),
        FVector(-4, 3, 1),
        FVector(2, 10, 1),
        FVector(1, 1, -1),
        FVector(2, -1, 1),
        FVector(4, -3, -1),
        FVector(-2, 10, 1)
    };
    for (int i = 0; i < checkup.size(); i++){
        FVector &a = checkup[i];
        a = a.GetSafeNormal();

        MMatrix rot = MMatrix::createRotatorFrom(a);
        FVector b(1, 0, 0);
        b = rot * b;

        FString message = FString::Printf(
            TEXT("debug matrix rotation from vector: (%.2f %.2f %.2f) -> (%.2f %.2f %.2f)"),
            a.X,
            a.Y,
            a.Z,
            b.X,
            b.Y,
            b.Z
        );
        DebugHelper::logMessage(message);
    }
    

    
}