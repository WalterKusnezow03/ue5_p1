// Fill out your copyright notice in the Description page of Project Settings.


#include "gameModeSubclass.h"

#include "p2/entityManager/referenceManager.h"
#include "p2/entityManager/EntityManager.h"
#include "Engine/World.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

#include "Engine/AssetManager.h"
#include "Engine/Blueprint.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/ConstructorHelpers.h"
#include "AssetPlugin/gamestart/AssetLoader.h"


// Game Launch
#include "p2/_world/EGameState.h"
#include "p2/_world/worldLevel.h"
#include "p2/_world/gameStateManager/GameStateManager.h"


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

}

// ----- DEFAULT BEGIN PLAY -----

void AgameModeSubclass::BeginPlay()
{
    Super::BeginPlay();

    //set worldLevel UWorld pointer for easier world pointer getting in non aactor classes
    worldLevel::SetWorld(GetWorld()); //needed for init!

    //load all assets
    AssetLoader a(GetWorld());
    a.loadAssets();

    //launch ui from here, not from player
    UPlayerUi::createNewInstance(GetWorld());

    //opens game launch screen
    worldLevel::gameStateManager.UpdateGameState(EGameState::EGameLaunchScreen);

    /// ---- TODO: PLAYER LÄDT / END LEVEL WITH UI ! ---- LOOP ÜBERLEGEN, FREE TERRAIn, FREE PATH FINDER!
    bool debugAllowWorldCreation = false;
    if (debugAllowWorldCreation)
    {
        /**
         * CREATE WORLD LEVEL
         *
         * must be in begin play to prevent bugs with asset spawning, NOT in constructor.
         */
        worldLevel::initWorld(GetWorld());
    }
}


void AgameModeSubclass::EndPlay(const EEndPlayReason::Type EndPlayReason){

    worldLevel::resetWorld(); //clears all instances of assets etc. All memory release
    DebugHelper::logMessage(TEXT("DEBUG END PLAY"));
}
