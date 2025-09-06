// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "p2/ui/screens/PlayerHud.h"
#include "p2/ui/screens/loadout/LoadoutScreen.h"

#include "p2/entityManager/referenceManager.h"

UPlayerUi *UPlayerUi::instancePtr = nullptr;

//instance maker with init call!
UPlayerUi* UPlayerUi::createNewInstance(UWorld *world){
    if(instancePtr){
        return instancePtr; //if not nullptr, the existing instance is returned
    }

    if(world != nullptr){
        
        UClass *blueprint = UPlayerUi::StaticClass();
        if(blueprint != nullptr){

            UPlayerUi* newUiInstance = CreateWidget<UPlayerUi>(world, blueprint);
            if (newUiInstance != nullptr)
            {
                newUiInstance->init(world);
                UPlayerUi::instancePtr = newUiInstance;
                return newUiInstance;
            }
        }
    }
    return nullptr;
}

UPlayerUi *UPlayerUi::currentInstance(){
    return instancePtr;
}

void UPlayerUi::BeginDestroy(){
    UPlayerUi::instancePtr = nullptr;
    Super::BeginDestroy();
}

//constructor like
void UPlayerUi::init(UWorld *world){
    if(isInited){
        return;
    }
    isInited = true;

    //Must be called to be added to viewport!
    Super::init(world);

    createBasePlayerHud();
    createPauseScreen();
    createLoadoutScreen();
    createGameLaunchScreen();
}

///@brief creates the player hud
void UPlayerUi::createBasePlayerHud(){
    playerHud = NewObject<UPlayerHud>(this);
    if(playerHud){
        playerHud->init(*this);
    }
    
}

void UPlayerUi::createPauseScreen(){
    pauseScreen = NewObject<UPauseScreen>(this);
    if(pauseScreen){
        pauseScreen->init(*this);
        pauseScreen->SetVisible(false);
    }
    
}

void UPlayerUi::createLoadoutScreen(){
    loadoutScreen = NewObject<ULoadoutScreen>(this);
    if(loadoutScreen){
        loadoutScreen->init(*this);
        loadoutScreen->SetVisible(false);
    }
}

void UPlayerUi::createGameLaunchScreen(){
    gameLaunchScreen = NewObject<UGameLaunchScreen>(this);
    if(gameLaunchScreen){
        gameLaunchScreen->init(*this);
        gameLaunchScreen->SetVisible(false);
    }
}



//public api ui update text

void UPlayerUi::updateMissionText(FString message){
    if(playerHud){
        playerHud->updateTopWaringElement(message);
    }
}

void UPlayerUi::updateMissionTextTimed(FString message){
    if(playerHud){
        float timetolive = 2.0f;
        playerHud->updateTopWarningElementTimed(message, timetolive);
    }
}




// --- player interact Open Screen api ---

void UPlayerUi::openPauseScreen(){
    if(screenSwitchAllowed()){
        if(playerHud){
            playerHud->SetVisible(false);
        }
        openedScreenStack.open(pauseScreen);
        showPlayerCursor(true);
    }
}

//called from Pause screen, AworldLevel::gameStateManager flag update needed!
void UPlayerUi::openGameScreen(){

    AworldLevel::gameStateManager.OverrideGameState(EGameState::EGamePlay);

    // close ui
    openedScreenStack.closeAll(); //very important
    if(playerHud){
        playerHud->SetVisible(true);
    }
    showPlayerCursor(false);

    if(loadoutScreen){
        loadoutScreen->reloadPlayerLoadoutOnUiLeaveIfNeeded();
    }

    
}

void UPlayerUi::openLoadoutScreen(){
    if(screenSwitchAllowed()){
        if(playerHud){
            playerHud->SetVisible(false);
        }
        openedScreenStack.open(loadoutScreen);
    }
}


void UPlayerUi::openGameLaunchScreen(){
    if(screenSwitchAllowed()){
        if(playerHud){
            playerHud->SetVisible(false);
        }
        openedScreenStack.open(gameLaunchScreen);
    }
    
}



bool UPlayerUi::screenSwitchAllowed(){
    return openedScreenStack.ScreenAlreadyOpen(gameLaunchScreen) == false;
}

void UPlayerUi::showPlayerCursor(bool show){
    referenceManager::showPlayerCursor(show);
}



// --- api for hud update ---
UPlayerHud *UPlayerUi::HudInstance(){
    if(playerHud){
        return playerHud;
    }
    return nullptr;
}
