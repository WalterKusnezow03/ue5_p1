// Fill out your copyright notice in the Description page of Project Settings.


#include "gameModeSubclass.h"

 #include "p2/entityManager/EntityManager.h"
#include "Engine/World.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

#include "Engine/AssetManager.h"
#include "Engine/Blueprint.h"
#include "UObject/SoftObjectPath.h"
#include "UObject/ConstructorHelpers.h"
#include "AssetPlugin/gamestart/AssetLoader.h"

#include "p2/_world/AdditionalAssetLoading/P2AssetLoader.h"

//debug
#include "AssetPlugin/gamestart/PathMaker/AssetPathMaker.h"

// Game Launch
#include "p2/_world/EGameState.h"
#include "p2/_world/worldLevel.h"
#include "p2/_world/gameStateManager/GameStateManager.h"

//gc
#include "GcGameCore/Launcher/GcLauncher.h"

//debug
#include "IkHumanoidModell/Ik/Controller/SLIP/liftOffFrame/SlipLiftOffFrameFinder.h"


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
    DebugHelper::DebugToolsEnabled = true;

    //DEBUG
    RunTests(); 


    //set worldLevel UWorld pointer for easier world pointer getting in non aactor classes
    AworldLevel::MakeInstance(GetWorld());
    //SetWorld(GetWorld()); //needed for init!

    AGcLauncher::MakeInstance(GetWorld());

    //load all assets (saved to asset manager single ton)
    //AssetLoader a;
    //a.loadAssets();

    P2AssetLoader loader;
    loader.LoadGameAssets();

    //launch ui from here, not from player
    //UPlayerUi::createNewInstance(GetWorld());
    

    //opens game launch screen -> launched by world on player ref set
    //AworldLevel::gameStateManager.UpdateGameState(EGameState::EGameLaunchScreen);

    /// ---- PLAYER LOADS / ENDS LEVEL WITH UI ! ----
    bool debugAllowWorldCreation = false;
    if (debugAllowWorldCreation)
    {
        /**
         * CREATE WORLD LEVEL
         *
         * must be in begin play to prevent bugs with asset spawning, NOT in constructor.
         */
        AworldLevel::initWorld(GetWorld());
    }

    //SlipLiftOffFrameFinder::Test();
}

void AgameModeSubclass::EndPlay(const EEndPlayReason::Type EndPlayReason){
    DebugHelper::logMessage(TEXT("DEBUG END PLAY"));
}




//---- tests -----
#include "CoreMath/algorithm/_Test/TestAlgorithm.h"
#include "GameCore/MeshGenBase/MeshData/BoundingBox/BoundingBoxSimple.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"
void AgameModeSubclass::RunTests(){
    //AssetPathMaker::Test();
    //TestAlgorithm::Test();
    //BoundingBoxSimple::Test();
    //ANNSocket::MakeInstance(GetWorld());
}