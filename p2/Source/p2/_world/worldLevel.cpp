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
#include "DebugPlugin/DebugHelper.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"

 #include "terrainPlugin/meshgen/rooms/doorLike/DoorBase.h"
#include "p2/aeroDynamics/AeroActor.h"
#include "GameCore/util/TVector.h"
#include "p2/vehicles/vehicle/vehicleCar.h"
#include "terrainPluginBase/BaseTerrainInterface/bezier/bezierCurve.h"
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
UiSimulation AworldLevel::uiSimulation;
//AworldLevel *AworldLevel::InstanceWorldLevel = nullptr;

AworldLevel::AworldLevel() : AworldLevelBase()
{   
}

void AworldLevel::MakeInstance(UWorld *world){
    if(world){
        AworldLevel *created = Make<AworldLevel>(world);
        Instance = created;
    }
}


void AworldLevel::BeginPlay(){
    Instance = this;
}

void AworldLevel::Tick(float deltatime){
    Super::Tick(deltatime);
    //Nothing to tick here
    createOutpostsRequested(); //stored in super class
}


void AworldLevel::OnPlayerReferenceSet(){
    Super::OnPlayerReferenceSet();
    if(PlayerPointer){
        AworldLevel::gameStateManager.UpdateGameState(EGameState::EGameLaunchScreen);
    }   
}



/// --- GAME CLOSE ALL ---
/// IS RESET FROM GAME MODE SUBCLASS, MUST BE CALLED ON END PLAY!
/// @brief clears all pointers -> call only on very begin or very end of level!
/// -> entity manager: holds all entities and room, terrain basic assets!
/// -> outpostManager: holds all outposts and assigns them on request
/// -> pathfinder singleton instance -> all nodes will be wiped
/// -> asset manager: all asset data wiped
void AworldLevel::EndPlay(const EEndPlayReason::Type EndPlayReason){
    //EndCurrentWorld();

    EntityManager::EndPlay(); // very important
    AlertManager::EndPlay();

    if(outpostManagerPointer != nullptr){
        delete outpostManagerPointer;
        outpostManagerPointer = nullptr;
    }
    terrainLauncher = nullptr;

    assetManager::EndGame(); //very important
    

    Instance = nullptr;

    Super::EndPlay(EndPlayReason);
}




/// @brief will init the terrain, keep in mind that all assets must be loaded before!
/// @param world 
void AworldLevel::initWorld(UWorld *world){
    if(!Instance){
        MakeInstance(world);
    }

    FString WorldName = TEXT("World1"); // MUST BE SET FROM ANOTHER LEVEL / CHOOSE LEVELS SCREEN
    initWorld(WorldName);
}

void AworldLevel::initWorld(FString WorldName){
    AworldLevel *instancePtr = AworldLevel::GetInstance();
    if (!instancePtr)
    {
        return;
    }
    instancePtr->EndCurrentWorld();
    DebugHelper::logMessage("AworldLevel::InitWorld ", WorldName);

    gameStateManager.UpdateGameState(EGameState::EGamePlay);

    EntityManager::BeginPlay(); //very important


    instancePtr->createTerrain(WorldName);

    //create rooms
    instancePtr->DebugCreateRooms();

    //edge collector must be added here later
    instancePtr->createPathFinder(WorldName);

    //creates one bot, BUT 5 humans will spawn if one outpost is created!
    int count = 1;
    instancePtr->humanBotsOnStart(count);

    //InstanceWorldLevel->createGroundPane();

    //testing
    DebugHelper::Debugtest(instancePtr->GetWorld());


    //bipedal locomotion 2.0 skelleton
    instancePtr->createBoneActorDebug();
    

    //debug plucker forward dynamics joints
    instancePtr->createPluckerBoneActorDebug();
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


//kill world
void AworldLevel::EndCurrentWorld(){
    FPathFinderModule::EndPathFinder();
    if(terrainLauncher){
        terrainLauncher->EndAndSave();
    }
    if(OutpostManager *ptr = outpostManager()){
        ptr->EndOutpostManager();
    }
    DebugHelper::logMessage("AworldLevel::EndWorld");
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
    if(AworldLevel *InstanceWorldLevel = GetInstance()){
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
        //return; //deprecated, auto unload reload world
    }
    if(AworldLevel *InstanceWorldLevel = GetInstance()){
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
        FVector spawnLocation(-1000, -1000, 100);
        for (int i = 0; i < count; i++){
            FVector spawnLocationCurrent = spawnLocation + FVector(0, count * 200,0);
            e->spawnHumanEntity(GetWorld(), spawnLocationCurrent, teamEnum::enemyTeam);
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





// Bipedal locomotion 2.0 debug skelleton
void AworldLevel::createBoneActorDebug(){
    UWorld *world = GetWorld();
    if(world){
        //AIkActor::CreateInstance(world);
        AIkDebugActor::CreateInstance(world);
    }
}


// Plucker forward dynamic debug 
#include "PlueckerCore/Debug/DebugJointsActor.h"
void AworldLevel::createPluckerBoneActorDebug(){
    FVector location(0, 0, 200);
    ADebugJointsActor::CreateInstance(GetWorld(), location);

}
