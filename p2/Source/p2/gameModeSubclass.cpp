// Fill out your copyright notice in the Description page of Project Settings.


#include "gameModeSubclass.h"

#include "p2/entityManager/referenceManager.h"
#include "pathFinding/PathFinder.h"
#include "pathFinding/EdgeCollector.h"
#include "entityManager/EntityManager.h"
#include "Engine/World.h"

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
    static ConstructorHelpers::FObjectFinder<UClass> EntityBPClass(
        TEXT("Blueprint'/Game/Prefabs/player/entityPrefab.entityPrefab_C'")
    );
    if (EntityBPClass.Succeeded())
    {
        UClass *bp = EntityBPClass.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setEntityUClassBp(bp);
        }
    }

    static ConstructorHelpers::FObjectFinder<UClass> humanBpClass(
        TEXT("Blueprint'/Game/Prefabs/player/humanEntityPrefab.humanEntityPrefab_C'")
    );
    if (humanBpClass.Succeeded())
    {
        UClass *bp = humanBpClass.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setHumanEntityUClassBp(bp);
        }
    }

}










/// @brief loads all weapons to the entity manager
/// @param entityManager entity manager instance
void AgameModeSubclass::loadWeapons(EntityManager *entityManager){

    //pistol
    static ConstructorHelpers::FObjectFinder<UClass> pistolBpClass(
        TEXT("Blueprint'/Game/Prefabs/Weapons/pistol/pistolNew/pistolNew.pistolNew_C'")
    );
    if (pistolBpClass.Succeeded())
    {
        UClass *bp = pistolBpClass.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setWeaponUClassBP(bp, weaponEnum::pistol);
        }
    }

    //assault rifle
    static ConstructorHelpers::FObjectFinder<UClass> rifleBpClass(
        TEXT("Blueprint'/Game/Prefabs/Weapons/rifle/rifleBp.rifleBp_C'")
    );
    if (rifleBpClass.Succeeded())
    {
        UClass *bp = rifleBpClass.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setWeaponUClassBP(bp, weaponEnum::assaultRifle);
        }
    }
}


/// @brief load thrower and throwables
/// @param entityManager entity manager
void AgameModeSubclass::loadThrower(EntityManager* entityManager){
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
    }
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

    static ConstructorHelpers::FObjectFinder<UClass> room1(
        TEXT("Blueprint'/Game/Prefabs/rooms/room1.room1_C'")
    );
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
}


void AgameModeSubclass::loadTerrain(EntityManager *entityManager){
    static ConstructorHelpers::FObjectFinder<UClass> emptyMeshActor(
        TEXT("Blueprint'/Game/Prefabs/terrain/emptyCustomMeshActor.emptyCustomMeshActor_C'")
    );
    if (emptyMeshActor.Succeeded())
    {
        UClass *bp = emptyMeshActor.Object;
        if(entityManager != nullptr && bp != nullptr){
            entityManager->setEmptyMeshUClassBp(bp);
        }
    }
}






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
}








// ----- DEFAULT METHODS -----
//will create edge collector and path finder instance on start!


void AgameModeSubclass::BeginPlay()
{
    Super::BeginPlay();

    // Perform actions that depend on the world being ready
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




            //test path finding needed!
            FVector a = FVector(0, 0, 0);
            FVector b = FVector(-2100, -1020, 80);

            //showPos(a, FColor::Red);
            //showPos(b, FColor::Blue);

            std::vector<FVector> path = p->getPath(a, b);

            showPath(path);
        }
    }
}


//shows the edges for debugging
void AgameModeSubclass::showEdges(std::vector<FVector> &edges){
    for (int i = 0; i < edges.size(); i++){
        FVector Start = edges.at(i);
        FVector End = edges.at(i) + FVector(0, 0, 1000);
        if (GetWorld())
        {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 100.0f, 0, 1.0f);
        }
        
        if (GEngine)
        {
            FString string = FString::Printf(TEXT("%d"), Start.X);
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, string);
        }
    }
}



void AgameModeSubclass::showPath(std::vector<FVector> &path){
    if (GEngine)
    {
        FString string = FString::Printf(TEXT("nodes to dest: %d"), path.size());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, string);
    }
    if(path.size() > 1){
        for (int i = 1; i < path.size(); i++){
            FVector Start = path.at(i);
            FVector End = path.at(i - 1);
            if (GetWorld())
            {
                DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 100.0f, 0, 1.0f);
            }
        }
    }
    
}


void AgameModeSubclass::showPos(FVector &v, FColor color){

    FVector Start = v;
    FVector End = v + FVector(0, 0, 2000);
    if (GetWorld())
    {
        DrawDebugLine(GetWorld(), Start, End, color, false, 100.0f, 0, 1.0f);
    }
    
}