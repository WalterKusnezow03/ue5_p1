// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader.h"

AssetLoader::AssetLoader(UWorld *worldIn)
{
    if(worldIn != nullptr){
        world = worldIn;
    }
}

AssetLoader::~AssetLoader()
{
}


//constructor
void AssetLoader::loadAssets()
{
   
    //load the entity manager
    EntityManager *entityManager = EntityManager::instance();



    loadEntities(entityManager);
    loadWeapons(entityManager);
    
    loadThrower(entityManager);
    loadParticles(entityManager);
    loadRooms(entityManager);
    loadTerrain(entityManager);
    loadMaterials(entityManager);

}


/// @brief passin your path and prefab blueprint name
/// @param path path like "/Game/Prefabs..."
/// @param bluePrintName "justTheName" of the file
/// @return FString path to load from
FString AssetLoader::buildPath(FString path, FString bluePrintName){
    FString pathFront = FString::Printf(TEXT("Blueprint'"));
    //FString room = FString::Printf(TEXT("room"));
    FString dot = FString::Printf(TEXT("."));
    FString _C_end = FString::Printf(TEXT("_C'"));
    
    FString connect = pathFront;
    connect.Append(path);
    connect.Append(bluePrintName);
    connect.Append(dot);
    connect.Append(bluePrintName);
    connect.Append(_C_end);
    return connect;
}




/// @brief load all entities 
/// @param entityManager entity manager to create
void AssetLoader::loadEntities(EntityManager *entityManager){

    if(entityManager){
        FString path = FString::Printf(TEXT("/Game/Prefabs/player/"));
        FString entityString = buildPath(path, "entityPrefab");
        entityManager->setEntityUClassBp(loadUClassBluePrint(entityString));

        //FString entityString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/entityPrefab.entityPrefab_C'"));
        //entityManager->setEntityUClassBp(loadUClassBluePrint(entityString));
        FString humanString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/humanEntityPrefab.humanEntityPrefab_C'"));
        entityManager->setHumanEntityUClassBp(loadUClassBluePrint(humanString));
    }

}



/// @brief Method to load a Blueprint class from a path.
/// @param path The path to the Blueprint class.
/// @return The loaded UClass, or nullptr if it fails.
/// works as expected
UClass* AssetLoader::loadUClassBluePrint(FString path){
    // Load the class object dynamically
    UClass* bpClass = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
    
    // Check if the class was loaded successfully
    if (bpClass != nullptr)
    {
        return bpClass;
    }
    return nullptr;
}

UMaterial *AssetLoader::loadMaterial(FString path){
    // Load the class object dynamically
    UMaterial* bpObject = LoadObject<UMaterial>(nullptr, *path);
    
    // Check if the class was loaded successfully
    if (bpObject != nullptr)
    {
        return bpObject;
    }
    return nullptr;
}









/// @brief loads all weapons to the entity manager
/// @param entityManager entity manager instance
void AssetLoader::loadWeapons(EntityManager *entityManager){

    //pistol
    FString pistolString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/Weapons/pistol/pistolNew/pistolNew.pistolNew_C'"));
    UClass *bp = loadUClassBluePrint(pistolString);
    if(entityManager != nullptr && bp != nullptr){
        entityManager->setWeaponUClassBP(bp, weaponEnum::pistol);
    }

    
    //assault rifle
    FString rifleString = FString::Printf(
        TEXT("Blueprint'/Game/Prefabs/Weapons/rifle/rifleBp.rifleBp_C'")
    );
    UClass *riflebp = loadUClassBluePrint(rifleString);
    if(entityManager != nullptr && riflebp != nullptr){
        entityManager->setWeaponUClassBP(riflebp, weaponEnum::assaultRifle);
    }



}


/// @brief load thrower and throwables
/// @param entityManager entity manager
void AssetLoader::loadThrower(EntityManager* entityManager){
    
    if(entityManager){
        entityManager->setDefaultThrowerClassBp(
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/defaultthrower.defaultthrower_C'"))
        );

        entityManager->setThrowableUClassBp(
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/grenadeBp.grenadeBp_C'")),
            throwableEnum::greneade_enum
        );
    }
    
    
}



/// @brief load particles for the entitymanager
/// @param entityManager to set in
void AssetLoader::loadParticles(EntityManager *entityManager){
    static ConstructorHelpers::FObjectFinder<UClass> smokeParticle(
        TEXT("Blueprint'/Game/Prefabs/particle/particleSmoke.particleSmoke_C'")
    );
    if (smokeParticle.Succeeded())
    {
        UClass *bp = smokeParticle.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setparticleBp(bp, particleEnum::smoke_enum);
        }
    }


    static ConstructorHelpers::FObjectFinder<UClass> fireParticle(
        TEXT("Blueprint'/Game/Prefabs/particle/particleFire.particleFire_C'")
    );
    if (fireParticle.Succeeded())
    {
        UClass *bp = fireParticle.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setparticleBp(bp, particleEnum::fire_enum);
        }
    }
}


// ---- LOAD ROOMS ----

void AssetLoader::loadRooms(EntityManager *entityManager){

    //needs uworld for rooms
    if(!entityManager){
        return;
    }

    //refacture
    FString path = FString::Printf(TEXT("/Game/Prefabs/rooms/"));
    FString bpNamePart = FString::Printf(TEXT("room"));
    for (int i = 1; i < 5; i++){
        FString bpNamePart2 = FString::Printf(TEXT("%d"), i);
        FString connected = bpNamePart + bpNamePart2;
        FString finalPath = buildPath(path, connected);
        entityManager->setRoomuClassBp(world, loadUClassBluePrint(finalPath));
    }
    
    /*
    // rooms are named with room0, room1 etc, integer counting up
    // FString pathFront = FStringPrintf(TEXT("Blueprint'/Game/Prefabs/rooms/room1.room1_C'")); //sample
    FString pathFront = FString::Printf(TEXT("Blueprint'/Game/Prefabs/rooms/"));
    FString room = FString::Printf(TEXT("room"));
    FString dot = FString::Printf(TEXT("."));
    FString _C_end = FString::Printf(TEXT("_C'"));
    for (int i = 1; i < 5; i++){
        FString connect = pathFront;
        connect.Append(room);
        connect.Append(TEXT("%d"),i);
        connect.Append(dot);
        connect.Append(room);
        connect.Append(TEXT("%d"),i);
        connect.Append(_C_end);
        if(entityManager){
            entityManager->setRoomuClassBp(world, loadUClassBluePrint(connect));
        }
    }

    //load rooms
    pathFront = FString::Printf(TEXT("Blueprint'/Game/Prefabs/rooms/walls/doors/"));
    FString door = FString::Printf(TEXT("door"));
    for (int i = 1; i < 2; i++){
        FString connect = pathFront;
        connect.Append(room);
        connect.Append(TEXT("%d"),i);
        connect.Append(dot);
        connect.Append(room);
        connect.Append(TEXT("%d"),i);
        connect.Append(_C_end);
    }*/

}


void AssetLoader::loadTerrain(EntityManager *entityManager){
   
    FString path = FString::Printf(TEXT("Blueprint'/Game/Prefabs/terrain/emptyCustomMeshActor.emptyCustomMeshActor_C'"));
    if(entityManager != nullptr){
        entityManager->setEmptyMeshUClassBp(loadUClassBluePrint(path));
    }
}

/// @brief loads all materials needed
/// @param entityManager 
void AssetLoader::loadMaterials(EntityManager *entityManager){
    if(entityManager != nullptr){

        entityManager->addMaterial(
            materialEnum::grassMaterial, 
            loadMaterial(TEXT("Blueprint'/Game/Prefabs/terrain/materials/grassMaterial.grassMaterial'"))
        );


    }
}





/**
 * ATTENTION: This is still called from game mode subclass, unclear whether called here or not
 */

void AssetLoader::createPathFinder(UWorld *WorldIn){
    if(WorldIn == nullptr){
        return;
    }

    if (WorldIn)
    {
        EdgeCollector c = EdgeCollector();
        std::vector<FVector> &edges = c.getAllEdges(WorldIn, 100.0f);

        //showEdges(edges);

        FString string = FString::Printf(TEXT("collected %d"), edges.size());
        DebugHelper::showScreenMessage(string);

        PathFinder *p = PathFinder::instance(WorldIn);
        if(p != nullptr){
            p->addNewNodeVector(edges);
        }
    }
}

