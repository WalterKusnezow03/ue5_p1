// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader.h"
#include "p2/gamestart/assetManager.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/_world/worldLevel.h"
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
    EntityManager *entityManager = worldLevel::entityManager();

    loadMaterials(); //materials need to be loaded first to not have any issues

    loadEntities();

    loadWeapons();
    loadThrower();
    loadParticles();

    loadTerrain(entityManager); //terrain needs to be loaded first because of the empty mesh actor needed for rooms too
    loadRooms(entityManager);

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
void AssetLoader::loadEntities(){

    if(assetManager *am = assetManager::instance()){
        FString path = FString::Printf(TEXT("/Game/Prefabs/player/"));
        FString entityString = buildPath(path, "entityPrefab");
        am->addBp(entityEnum::entity_enum, loadUClassBluePrint(entityString));


        FString humanString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/humanEntityPrefab.humanEntityPrefab_C'"));
        am->addBp(entityEnum::human_enum, loadUClassBluePrint(humanString));
    
    }


    /*
    if(entityManager){
        FString path = FString::Printf(TEXT("/Game/Prefabs/player/"));
        FString entityString = buildPath(path, "entityPrefab");
        entityManager->setEntityUClassBp(loadUClassBluePrint(entityString));

        //FString entityString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/entityPrefab.entityPrefab_C'"));
        //entityManager->setEntityUClassBp(loadUClassBluePrint(entityString));
        FString humanString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/humanEntityPrefab.humanEntityPrefab_C'"));
        entityManager->setHumanEntityUClassBp(loadUClassBluePrint(humanString));
    }*/

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
void AssetLoader::loadWeapons(){

    if(assetManager *a = assetManager::instance()){
        
        //pistol
        FString pistolString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/Weapons/pistol/pistolNew/pistolNew.pistolNew_C'"));
        UClass *bp = loadUClassBluePrint(pistolString);
        a->addBp(weaponEnum::pistol, bp);

        
        //ar
        FString rifleString = FString::Printf(
            TEXT("Blueprint'/Game/Prefabs/Weapons/rifle/rifleBp.rifleBp_C'")
        );
        UClass *riflebp = loadUClassBluePrint(rifleString);
        a->addBp(weaponEnum::assaultRifle, riflebp);

        //thrower
        UClass *throwerBp = loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/defaultthrower.defaultthrower_C'"));
        a->addBp(weaponEnum::thrower, throwerBp);

    }



}


/// @brief load thrower and throwables
/// @param entityManager entity manager
void AssetLoader::loadThrower(){
    
    
    if(assetManager *a = assetManager::instance()){

        a->addBp(
            throwableEnum::greneade_enum,
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/grenadeBp.grenadeBp_C'"))
        );
    }
    
}



/// @brief load particles for the entitymanager
/// @param entityManager to set in
void AssetLoader::loadParticles(){
    
    if(assetManager *am = assetManager::instance()){

        am->addBp(
            particleEnum::smoke_enum, 
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/particle/particleSmoke.particleSmoke_C'"))
        );

        am->addBp(
            particleEnum::fire_enum, 
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/particle/particleFire.particleFire_C'"))
        );

        am->addBp(
            particleEnum::particleNone_enum, 
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/particle/particleNone.particleNone_C'"))
        );
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
    for (int i = 1; i <= 9; i++){
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
void AssetLoader::loadMaterials(){
    
    if(assetManager *a = assetManager::instance()){
        //grass material
        a->addMaterial(
            materialEnum::grassMaterial,
            loadMaterial(TEXT("Blueprint'/Game/Prefabs/terrain/materials/grassMaterial.grassMaterial'"))
        );
        
        //stone material
        a->addMaterial(
            materialEnum::stoneMaterial,
            loadMaterial(TEXT("Blueprint'/Game/Prefabs/terrain/materials/stoneMaterial.stoneMaterial'"))
        );

        //wall material
        a->addMaterial(
            materialEnum::wallMaterial,
            loadMaterial(TEXT("Blueprint'/Game/Prefabs/rooms/materials/wallMaterial.wallMaterial'"))
        );

        //glass material
        a->addMaterial(
            materialEnum::glassMaterial,
            loadMaterial(TEXT("Blueprint'/Game/Prefabs/rooms/materials/glassMaterial.glassMaterial'"))
        );
    }

}



