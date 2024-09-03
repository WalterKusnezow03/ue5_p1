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



//constructor
AgameModeSubclass::AgameModeSubclass()
{
    // Set default pawn class to your Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Prefabs/player/playerPrefab")); // Adjust path if necessary
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // Set default pawn class
    //DefaultPawnClass = AplayerScript::StaticClass();

    //trying asset loader
    AssetLoader a(GetWorld());
    a.loadAssets();
    return;


}


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







