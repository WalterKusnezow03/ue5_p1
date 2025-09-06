// Fill out your copyright notice in the Description page of Project Settings.


#include "worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/OutPost/OutpostManager.h"
#include "p2/entityManager/AlertManager.h"

#include "terrainPlugin/meshgen/rooms/layoutCreator/layoutMaker.h"
#include "GameCore/util/TVector.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/terrainCreator.h"
#include "terrainPlugin/meshgen/foliage/MatrixTree/MatrixTree.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "terrainPlugin/meshgen/foliage/MatrixTree/ETreeType.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "p2/meshgen/specialMeshactors/wingsuitMeshActor.h"
#include "terrainPlugin/meshgen/foliage/rocks/RockCreator.h"
#include "terrainPlugin/meshgen/water/customWaterActor.h"
#include "terrainPlugin/meshgen/rooms/roomActor/roomProcedural.h"
#include "GameCore/DebugHelper.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"

#include "p2/entityManager/referenceManager.h"
#include "terrainPlugin/meshgen/rooms/doorLike/DoorBase.h"
#include "p2/aeroDynamics/AeroActor.h"
#include "GameCore/util/TVector.h"
#include "p2/vehicles/vehicle/vehicleCar.h"
#include "terrainPlugin/meshgen/generation/bezierCurve.h"
#include "p2/entityManager/OutPost/Outpost.h"


#include "PathFinder/Public/PathFinderModule.h"
#include "IkHumanoidModell/actor/IkDebugActor.h"

//testing storage plugin
#include "StoragePlugin/Storage/Test/TestStorageInterface.h"
#include "terrainPlugin/Storage/chunkMapHeaderLoading/ChunkMapStorageInterface.h"



#include "terrainPlugin/main/TerrainLauncher.h"




#include "CoreMinimal.h"

template class TVector<FVector2D>;




//static vars init:
GameStateManager AworldLevel::gameStateManager;
PlayerStatusManager AworldLevel::playerStatusManager;
AworldLevel *AworldLevel::InstanceWorldLevel = nullptr;

AworldLevel::AworldLevel() : AworldLevelBase()
{   
}

void AworldLevel::MakeInstance(UWorld *world){
    if(world){
        AworldLevel *created = Make<AworldLevel>(world);
    }
}


void AworldLevel::BeginPlay(){
    InstanceWorldLevel = this;
}

void AworldLevel::Tick(float deltatime){
    Super::Tick(deltatime);
    //Nothing to tick here
    createOutpostsRequested(); //stored in super class
}



/// --- GAME CLOSE ALL ---
/// IS RESET FROM GAME MODE SUBCLASS, MUST BE CALLED ON END PLAY!
/// @brief clears all pointers -> call only on very begin or very end of level!
/// -> entity manager: holds all entities and room, terrain basic assets!
/// -> outpostManager: holds all outposts and assigns them on request
/// -> pathfinder singleton instance -> all nodes will be wiped
/// -> asset manager: all asset data wiped
void AworldLevel::EndPlay(const EEndPlayReason::Type EndPlayReason){
    
    EntityManager::EndPlay(); // very important
    AlertManager::EndPlay();

    if(outpostManagerPointer != nullptr){
        delete outpostManagerPointer;
        outpostManagerPointer = nullptr;
    }
    terrainLauncher = nullptr;

    assetManager::EndGame(); //very important
    referenceManager::EndPlay();

    InstanceWorldLevel = nullptr;

    Super::EndPlay(EndPlayReason);
}




/// @brief will init the terrain, keep in mind that all assets must be loaded before!
/// @param world 
void AworldLevel::initWorld(UWorld *world){
    if(!InstanceWorldLevel){
        MakeInstance(world);
    }

    FString WorldName = TEXT("World1"); // MUST BE SET FROM ANOTHER LEVEL / CHOOSE LEVELS SCREEN
    initWorld(WorldName);
}

void AworldLevel::initWorld(FString WorldName){
    if(!InstanceWorldLevel){
        return; 
    }

    gameStateManager.UpdateGameState(EGameState::EGamePlay);

    EntityManager::BeginPlay(); //very important


    InstanceWorldLevel->createTerrain(WorldName);

    //create rooms
    InstanceWorldLevel->DebugCreateRooms();

    //edge collector must be added here later
    InstanceWorldLevel->createPathFinder(WorldName);

    //creates one bot, BUT 5 humans will spawn if one outpost is created!
    InstanceWorldLevel->humanBotsOnStart(1);
        
    InstanceWorldLevel->createGroundPane();

    //testing
    DebugHelper::Debugtest(InstanceWorldLevel->GetWorld());

    //debugBezier();
    //debugAngleFinder();
    //debugCreateWater();
    //debugCreateRock();
    //debugCreateWingsuitMesh();

    //InstanceWorldLevel->debugMatrix();


    //DebugCreatedoor();
    //debugBezier();

    //createAeroActor();
    //createCar();

    //createJointActor();

    InstanceWorldLevel->createBoneActorDebug();
    InstanceWorldLevel->debugStoragePlugin();
}




void AworldLevel::createOutpostsRequested(){
    if(bBlockOutPostCreation){
        return;
    }

    for (int i = 0; i < outpostsToCreate.Num(); i++){
        FVector &locationCurrent = outpostsToCreate[i];
        if (OutpostManager *ptr = outpostManager())
        {
            AOutpost *outpost = ptr->requestOutpost(GetWorld(), locationCurrent);
            if (outpost)
            {
                outpost->createAlarmPolesIfNeeded();
            }
        }
    }
    outpostsToCreate.Empty();
}

/**
 * ATTENTION: PathFinder Collect edges will only be called from this class and only once on level start
 */
void AworldLevel::createPathFinder(FString worldName){
    FPathFinderModule::StartPathFinder(GetWorld(), worldName);
}



/**
 * 
 * INSTANCE POINTERS
 * 
 */



/// @brief intended for one time use only, do not delete, do not save
/// @return returns pointer to entitymanager
EntityManager *AworldLevel::entityManager(){
    return EntityManager::instance();
}

/// @brief returns the outpost manager to ask for the nearest outpost
/// @return will return the pointer
OutpostManager * AworldLevel::outpostManager(){
    if(InstanceWorldLevel){
        if(InstanceWorldLevel->outpostManagerPointer == nullptr){
            InstanceWorldLevel->outpostManagerPointer = new OutpostManager();
        }
        return InstanceWorldLevel->outpostManagerPointer;
    }
    return nullptr;
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
void AworldLevel::createTerrain(FString worldName){
    if(isTerrainInited){
        return;
    }
    UWorld *world = InstanceWorldLevel->GetWorld();
    if(world != nullptr){
        isTerrainInited = true;
        if(terrainLauncher == nullptr){
            terrainLauncher = ATerrainLauncher::makeInstance(world, worldName);
        }else{
            //if a world switch is wanted the terrain launcher is not killed
            terrainLauncher->BeginAndLoad(worldName);
        }
    }
}




/** 
 * 
 * DEBUG HUMAN ENTITIES
 * 
*/
void AworldLevel::humanBotsOnStart(int count){
    if(bBlockEntities){
        return;
    }

    
    if(areBotsInited){
        return;
    }
    areBotsInited = true;

    EntityManager *e = entityManager();
    if (e != nullptr)
    {
        for (int i = 0; i < count; i++){
            FVector spawnLocation(-1000, -1000, 100);
            e->spawnHumanEntity(GetWorld(), spawnLocation, teamEnum::enemyTeam);
        }

        
    }
}






//debug method create rooms near world origin
void AworldLevel::DebugCreateRooms(){
    UWorld *world = GetWorld();
    if (world == nullptr)
    {
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








void AworldLevel::debugAngleFinder(){

    //return;
    UWorld *world = GetWorld();
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

    std::vector<FVector2D> output = AworldLevel::findAngles(lenghtAll, bones);

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

std::vector<FVector2D> AworldLevel::findAngles(float lengthAll, std::vector<float> &bones){
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





void AworldLevel::createGroundPane(){
    UWorld *world = GetWorld();
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


void AworldLevel::debugCreateWater(){
    return; //DEBUG

    UWorld *world = GetWorld();
    if(world != nullptr){

        int scaleMeters = 50 * 100;
        AcustomWaterActor::createWaterPane(
            world,
            FVector(-1000,-1000,0),
            scaleMeters
        );
    }
}




void AworldLevel::debugCreateRock(){
    UWorld *world = GetWorld();
    if(world != nullptr){
        EntityManager *pointer = AworldLevel::entityManager();
        if(pointer != nullptr){

            RockCreator rock;
            MeshData meshData = rock.createMesh();

            meshData = FVectorShape::createSphere(150, 50, true); //NEW DEBUG, fac outside

            FVector location(-2000, 0, 100);
            AcustomMeshActor *actor = pointer->spawnAcustomMeshActor(world, location);

            if(actor != nullptr){
                actor->replaceMeshData(meshData, materialEnum::stoneMaterial);
                actor->ReloadMeshAndApplyAllMaterials();
            }
        }
    }
}



void AworldLevel::debugCreateWingsuitMesh(){
    UWorld *world = GetWorld();
    if (world != nullptr)
    {
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





void AworldLevel::debugMatrix(){

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







void AworldLevel::debugBezier(){
    /*
    bezierCurve curve;

    FVector2D startingPoint;
    TVector<FVector2D> output;
    float _einheitsValue = 10.0f;
    float distanceBetweenAnchorsMin = 200.0f;
    float distanceBetweenAnchorsMax = 400.0f;
    float max_xy_coordinate = 10000.0f;

    curve.createNewRandomCurve(
        startingPoint,
        output,
        _einheitsValue,
        distanceBetweenAnchorsMin,
        distanceBetweenAnchorsMax,
        max_xy_coordinate
    );

    FVector prev;
    for(int i = 0; i < output.size(); i++){
        FVector2D &ref = output[i];
        FVector to3d(
            ref.X,
            0.0f,
            ref.Y
        );
        DebugHelper::showLineBetween(GetWorld(), prev, to3d, FColor::Blue);
        prev = to3d;
    }*/


}





void AworldLevel::DebugCreatedoor(){
    UWorld *world = GetWorld();
    if(world){
        FVector location(0, -1000, 0);
        ADoorBase *door = ADoorBase::Construct(world, location);
        if(door){
            //
        }
    }
}




void AworldLevel::createAeroActor(){
    UWorld *world = GetWorld();
    if(world){
        FVector location(500, -1000, 200);

        DebugHelper::showLineBetween(
            world,
            location,
            location + FVector(0, 0, 10000),
            FColor::Red,
            100.0f
        );
        DebugHelper::showLineBetween(
            world,
            location,
            FVector(0,0,0),
            FColor::Red,
            100.0f
        );

        AAeroActor *ptr = AAeroActor::Construct(world, location);
    }
}




void AworldLevel::createCar(){
    UWorld *world = GetWorld();
    if(world){
        AvehicleCar *car = AvehicleCar::Construct(world);
    }
}






void AworldLevel::createBoneActorDebug(){
    UWorld *world = GetWorld();
    if(world){
        //AIkActor::CreateInstance(world);
        AIkDebugActor::CreateInstance(world);
    }
}




void AworldLevel::debugStoragePlugin(){

    //all inner plugin types
    TestStorageInterface interfacePlugin;
    interfacePlugin.Test();

    //other
    ChunkMapStorageInterface storageMap;
    storageMap.Test();
}




