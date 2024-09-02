// Fill out your copyright notice in the Description page of Project Settings.


#include "gameModeSubclass.h"

#include "p2/entityManager/referenceManager.h"
#include "pathFinding/PathFinder.h"
#include "pathFinding/EdgeCollector.h"
#include "entityManager/EntityManager.h"
#include "Engine/World.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

#include "Engine/AssetManager.h"
#include "Engine/Blueprint.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/ConstructorHelpers.h"



//constructor
AgameModeSubclass::AgameModeSubclass()
{
    // Set default pawn class to your Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Prefabs/player/playerPrefab")); // Adjust path if necessary
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // Set default pawn class
    //DefaultPawnClass = AplayerScript::StaticClass();

    //load the entity manager
    EntityManager *entityManager = EntityManager::instance();



    loadEntities(entityManager);
    loadWeapons(entityManager);
    
    loadThrower(entityManager);
    loadParticles(entityManager);
    loadRooms(entityManager);
    loadTerrain(entityManager);

    
}

/// @brief load all entities 
/// @param entityManager entity manager to create
void AgameModeSubclass::loadEntities(EntityManager *entityManager){

    if(entityManager){
        FString entityString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/entityPrefab.entityPrefab_C'"));
        entityManager->setEntityUClassBp(loadUClassBluePrint(entityString));
        FString humanString = FString::Printf(TEXT("Blueprint'/Game/Prefabs/player/humanEntityPrefab.humanEntityPrefab_C'"));
        entityManager->setHumanEntityUClassBp(loadUClassBluePrint(humanString));
    }

}










/// @brief loads all weapons to the entity manager
/// @param entityManager entity manager instance
void AgameModeSubclass::loadWeapons(EntityManager *entityManager){

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



    /*
    static ConstructorHelpers::FObjectFinder<UClass>
        pistolBpClass(
            TEXT("Blueprint'/Game/Prefabs/Weapons/pistol/pistolNew/pistolNew.pistolNew_C'"));
    if (pistolBpClass.Succeeded())
    {
        UClass *bp = pistolBpClass.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setWeaponUClassBP(bp, weaponEnum::pistol);
        }
    }*/
    /*
    static ConstructorHelpers::FObjectFinder<UClass> rifleBpClass(
        TEXT("Blueprint'/Game/Prefabs/Weapons/rifle/rifleBp.rifleBp_C'")
    );
    if (rifleBpClass.Succeeded())
    {
        UClass *bp = rifleBpClass.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setWeaponUClassBP(bp, weaponEnum::assaultRifle);
        }
    }*/
}


/// @brief load thrower and throwables
/// @param entityManager entity manager
void AgameModeSubclass::loadThrower(EntityManager* entityManager){
    
    if(entityManager){
        entityManager->setDefaultThrowerClassBp(
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/defaultthrower.defaultthrower_C'"))
        );

        entityManager->setThrowableUClassBp(
            loadUClassBluePrint(TEXT("Blueprint'/Game/Prefabs/Throwables/grenadeBp.grenadeBp_C'")),
            throwableEnum::greneade_enum
        );
    }
    
    
    
    /*
    //thrower default
    static ConstructorHelpers::FObjectFinder<UClass> throwerDefault(
        TEXT("Blueprint'/Game/Prefabs/Throwables/defaultthrower.defaultthrower_C'")
    );
    if (throwerDefault.Succeeded())
    {
        UClass *bp = throwerDefault.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setDefaultThrowerClassBp(bp);
        }
    }

    //throwables
    static ConstructorHelpers::FObjectFinder<UClass> grenadeBpClass(
        TEXT("Blueprint'/Game/Prefabs/Throwables/grenadeBp.grenadeBp_C'")
    );
    if (grenadeBpClass.Succeeded())
    {
        UClass *bp = grenadeBpClass.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setThrowableUClassBp(bp, throwableEnum::greneade_enum);
        }
    }*/
}



/// @brief load particles for the entitymanager
/// @param entityManager to set in
void AgameModeSubclass::loadParticles(EntityManager *entityManager){
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

void AgameModeSubclass::loadRooms(EntityManager *entityManager){

    //needs uworld for rooms
    UWorld *world = GetWorld();

    //rooms are named with room0, room1 etc, integer counting up
    //FString pathFront = FStringPrintf(TEXT("Blueprint'/Game/Prefabs/rooms/room1.room1_C'")); //sample
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

    /*
    static ConstructorHelpers::FObjectFinder<UClass> room1(
            TEXT("Blueprint'/Game/Prefabs/rooms/room1.room1_C'"));
    if (room1.Succeeded())
    {
        UClass *bp = room1.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setRoomuClassBp(world, bp);
        }
    }

    static ConstructorHelpers::FObjectFinder<UClass> room2(
        TEXT("Blueprint'/Game/Prefabs/rooms/room2.room2_C'")
    );
    if (room2.Succeeded())
    {
        UClass *bp = room2.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setRoomuClassBp(world, bp);
        }
    }

    static ConstructorHelpers::FObjectFinder<UClass> room3(
        TEXT("Blueprint'/Game/Prefabs/rooms/room3.room3_C'")
    );
    if (room3.Succeeded())
    {
        UClass *bp = room3.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setRoomuClassBp(world, bp);
        }
    }

    static ConstructorHelpers::FObjectFinder<UClass> room4(
        TEXT("Blueprint'/Game/Prefabs/rooms/room4.room4_C'")
    );
    if (room4.Succeeded())
    {
        UClass *bp = room4.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setRoomuClassBp(world, bp);
        }
    }
    */
}


void AgameModeSubclass::loadTerrain(EntityManager *entityManager){
   
    FString path = FString::Printf(TEXT("Blueprint'/Game/Prefabs/terrain/emptyCustomMeshActor.emptyCustomMeshActor_C'"));
    if(entityManager != nullptr){
        entityManager->setEmptyMeshUClassBp(loadUClassBluePrint(path));
    }
}






/// @brief Method to load a Blueprint class from a path.
/// @param path The path to the Blueprint class.
/// @return The loaded UClass, or nullptr if it fails.
/// works as expected
UClass* AgameModeSubclass::loadUClassBluePrint(FString path){
    // Load the class object dynamically
    UClass* bpClass = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
    
    // Check if the class was loaded successfully
    if (bpClass != nullptr)
    {
        return bpClass;
    }
    return nullptr;
}

UMaterial *AgameModeSubclass::loadMaterial(FString path){
    // Load the class object dynamically
    UMaterial* bpObject = LoadObject<UMaterial>(nullptr, *path);
    
    // Check if the class was loaded successfully
    if (bpObject != nullptr)
    {
        return bpObject;
    }
    return nullptr;
}




/*
// METHOD HAS ISSUES AND DOESNT WORK PROPERLY!!
/// @brief method to load blue print class from a path
/// @param path 
/// @return 
UClass* AgameModeSubclass::loadUClassBluePrint(FString path){
    static ConstructorHelpers::FObjectFinder<UClass> bpUClass(
        *path // Convert FString to const TCHAR*
    );
    if (bpUClass.Succeeded())
    {
        UClass *bp = bpUClass.Object;
        if(bp != nullptr){
            return bp;
        }
    }
    return nullptr;
}*/


// ----- DEFAULT METHODS -----
//will create edge collector and path finder instance on start!


void AgameModeSubclass::BeginPlay()
{
    Super::BeginPlay();

    // Perform actions that depend on the world being ready
    //loads all edegs and creates the path finder on start
    createPathFinder();
}

void AgameModeSubclass::createPathFinder(){

    GLog->Flush();

    //get the world context
    UWorld* World = GetWorld();
    if (World)
    {
        EdgeCollector c = EdgeCollector();
        std::vector<FVector> &edges = c.getAllEdges(World, 100.0f);

        //showEdges(edges);

        FString string = FString::Printf(TEXT("collected %d"), edges.size());

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, string);
        }

        PathFinder *p = PathFinder::instance(World);
        if(p != nullptr){
            p->addNewNodeVector(edges);
        }
    }
}







