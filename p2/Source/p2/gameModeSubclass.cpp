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

    //worldLevel::resetWorld();

    /**
     * ASSET LOADER
     */
    AssetLoader a(GetWorld());
    a.loadAssets();
    

}

// ----- DEFAULT METHODS -----
//will create edge collector and path finder instance on start!


void AgameModeSubclass::BeginPlay()
{
    Super::BeginPlay();

    /**
     * CREATE WORLD LEVEL 
     * 
     * must be moved to begin play to prevent bugs with asset spawning
     */
    worldLevel::initWorld(GetWorld());
}


void AgameModeSubclass::EndPlay(const EEndPlayReason::Type EndPlayReason){

    worldLevel::resetWorld();
    FString s = FString::Printf(TEXT("DEBUG END PLAY"));
    DebugHelper::logMessage(s);
}
