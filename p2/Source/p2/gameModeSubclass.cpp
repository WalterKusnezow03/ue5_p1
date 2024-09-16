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
#include "p2/gameStart/AssetLoader.h"
#include "p2/_world/worldLevel.h"



//constructor
AgameModeSubclass::AgameModeSubclass()
{
    /**
     * SET PLAYER PAWN
     */
    // Set default pawn class to your Blueprinted character
    // Adjust path if necessary
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Prefabs/player/playerPrefab")); 
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    worldLevel::resetWorld(GetWorld());

    /**
     * ASSET LOADER
     */
    AssetLoader a(GetWorld());
    a.loadAssets();
    



    //remember at some point the level must be reset too: (maybe just before that is a good approach!)
    
    /**
     * CREATE WORLD LEVEL 
     */
    worldLevel::initWorld(GetWorld());

   
}

// ----- DEFAULT METHODS -----
//will create edge collector and path finder instance on start!


void AgameModeSubclass::BeginPlay()
{
    Super::BeginPlay();

    // Perform actions that depend on the world being ready
    //loads all edegs and creates the path finder on start
    //createPathFinder();
}

void AgameModeSubclass::createPathFinder(){

    GLog->Flush();

    //get the world context
    UWorld* World = GetWorld();
    if (World)
    {
        EdgeCollector c = EdgeCollector();
        //std::vector<FVector> &edges = 
        c.getAllEdges(World);

        return;
        /*
        //showEdges(edges);

        FString string = FString::Printf(TEXT("collected %d"), edges.size());

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, string);
        }

        PathFinder *p = PathFinder::instance(World);
        if(p != nullptr){
            p->addNewNodeVector(edges);
        }*/
    }
}







