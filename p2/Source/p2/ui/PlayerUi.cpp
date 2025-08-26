// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "p2/ui/screens/PlayerHud.h"
#include "p2/ui/screens/loadout/LoadoutScreen.h"

#include "p2/entityManager/referenceManager.h"

//instance maker with init call!
UPlayerUi* UPlayerUi::createNewInstance(UWorld *world){
    if(world != nullptr){
        
        UClass *blueprint = UPlayerUi::StaticClass();
        if(blueprint != nullptr){

            UPlayerUi* newUiInstance = CreateWidget<UPlayerUi>(world, blueprint);
            if (newUiInstance != nullptr)
            {
                newUiInstance->init(world);
                return newUiInstance;
            }
        }
    }
    return nullptr;
}



//constructor like
void UPlayerUi::init(UWorld *world){
    if(isInited){
        return;
    }
    isInited = true;

    //the root widget must be a canvas panel in bp!
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
        pauseScreen->setVisible(false);
    }
    
}

void UPlayerUi::createLoadoutScreen(){
    loadoutScreen = NewObject<ULoadoutScreen>(this);
    if(loadoutScreen){
        loadoutScreen->init(*this);
        loadoutScreen->setVisible(false);
    }
}

void UPlayerUi::createGameLaunchScreen(){
    gameLaunchScreen = NewObject<UGameLaunchScreen>(this);
    if(gameLaunchScreen){
        gameLaunchScreen->init(*this);
        gameLaunchScreen->setVisible(false);
    }
}



//public api ui update text

void UPlayerUi::updateAmmunitionText(int number){
    if(playerHud){
        playerHud->updateAmmunitionText(number);
    }
}

void UPlayerUi::updateAmmunitionText(FString message){
    if(playerHud){
        playerHud->updateAmmunitionText(message);
    }
}

void UPlayerUi::updateHealthText(int health){
    if(playerHud){
        playerHud->updateHealthText(health);
    }
}

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
    if(playerHud){
        playerHud->setVisible(false);
    }
    openedScreenStack.open(pauseScreen);
    showPlayerCursor(true);
}

void UPlayerUi::openGameScreen(){
    openedScreenStack.closeAll();
    if(playerHud){
        playerHud->setVisible(true);
    }
    showPlayerCursor(false);

    if(loadoutScreen){
        loadoutScreen->reloadPlayerLoadoutOnUiLeaveIfNeeded();
    }

    //the exit button doesnt do that on its own. Setting the flag to false
    //is okay.
    referenceManager::forceSetPlayerPaused(false);
}

void UPlayerUi::openLoadoutScreen(){
    if(playerHud){
        playerHud->setVisible(false);
    }
    openedScreenStack.open(loadoutScreen);
}


void UPlayerUi::openGameLaunchScreen(){
    if(playerHud){
        playerHud->setVisible(false);
    }
    openedScreenStack.open(gameLaunchScreen);
}






void UPlayerUi::showPlayerCursor(bool show){
    referenceManager::showPlayerCursor(show);
}






