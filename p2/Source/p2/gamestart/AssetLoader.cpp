// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader.h"
#include "p2/gamestart/assetManager.h"
#include "p2/gamestart/assetEnums/rooms/roomAssetEnum.h"

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

    cleanUpPath(connect);

    return connect;
}


void AssetLoader::cleanUpPath(FString &s){
    FString out;
    out.Reserve(s.Len());
    TCHAR slash = TEXT('/');
    TArray<TCHAR> asArray = s.GetCharArray();

    bool found = false;
    int i = 0;
    while(i  < asArray.Num()){
        if(asArray[i] != slash){
            out.AppendChar(asArray[i]);
            found = false;
        }
        else
        {
            if(!found){ //skip any else
                out.AppendChar(slash);
            }
            found = true;
        }
        i++;
    }
    s = out;
    //return out;
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
        return bpClass; //issues can still occur here when a non blueprint is found but treated like one!
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

    //load rooms
    FString path = FString::Printf(TEXT("/Game/Prefabs/rooms/"));
    FString bpNamePart = FString::Printf(TEXT("room"));
    for (int i = 1; i <= 8; i++){
        FString bpNamePart2 = FString::Printf(TEXT("%d"), i);
        FString connected = bpNamePart + bpNamePart2;
        FString finalPath = buildPath(path, connected);

        
        entityManager->setRoomuClassBp(world, loadUClassBluePrint(finalPath));
    }
    

    //load door
    path = FString::Printf(TEXT("/Game/Prefabs/rooms/walls/doors/"));
    FString _door = FString::Printf(TEXT("doorBp"));
    FString _num = FString::Printf(TEXT("%d"), 1);
    FString _connect = _door + _num;
    FString _builded = buildPath(path, _connect);
    if(assetManager *a = assetManager::instance()){
        a->addBp(roomAssetEnum::doorEnum, loadUClassBluePrint(_builded));
    }

    /*
    for (int i = 1; i < 2; i++){
        FString num = FString::Printf(TEXT("%d"), i);
        FString connect = door + num;
        FString builded = buildPath(path, connect);
        entityManager->setDooruClassBp(loadUClassBluePrint(builded));
    }*/

    //load wall
    path = FString::Printf(TEXT("/Game/Prefabs/rooms/walls/walls2/"));
    FString _wall = FString::Printf(TEXT("basicWallBp"));
    _builded = buildPath(path, _wall);
    if(assetManager *a = assetManager::instance()){
        a->addBp(roomAssetEnum::wallEnum, loadUClassBluePrint(_builded));
    }




    //load windows
    path = FString::Printf(TEXT("/Game/Prefabs/rooms/walls/windows/"));
    FString window = FString::Printf(TEXT("windowBp"));
    _num = FString::Printf(TEXT("%d"), 1);
    _connect = window + _num;
    _builded = buildPath(path, _connect);
    if(assetManager *a = assetManager::instance()){
        a->addBp(roomAssetEnum::windowEnum, loadUClassBluePrint(_builded));
    }
    //entityManager->setWindowuClassBp(loadUClassBluePrint(builded));
    /*for (int i = 1; i < 2; i++){
        FString num = FString::Printf(TEXT("%d"), i);
        FString connect = window + num;
        FString builded = buildPath(path, connect);
        entityManager->setWindowuClassBp(loadUClassBluePrint(builded));
    }*/
}


// ---- LOAD TERRAIN ----

/// @brief loads the terrain data
/// @param entityManager 
void AssetLoader::loadTerrain(EntityManager *entityManager){
   
    FString path = FString::Printf(TEXT("Blueprint'/Game/Prefabs/terrain/emptyCustomMeshActor.emptyCustomMeshActor_C'"));
    if(entityManager != nullptr){
        entityManager->setEmptyMeshUClassBp(loadUClassBluePrint(path));
    }
}

// ---- LOAD MATERIALS ----

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

