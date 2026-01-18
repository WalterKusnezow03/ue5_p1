// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "AssetEnumCollection/assetEnums/textureEnum.h"
#include "p2/ui/screens/PlayerHud.h"
#include "p2/ui/screens/loadout/LoadoutScreen.h"

#include "p2/player/playerScript.h"
#include "p2/ui/3Dui/LoadOutRoom/LoadoutRoomActor.h"

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




// --- player interact Open Screen api ---

void UPlayerUi::openPauseScreen(){
    DebugHelper::logMessage("UPlayerUi try open pause");
    if (screenSwitchAllowed())
    {
        //if(playerHud){
        //    playerHud->SetVisible(false);
        //}
        openedScreenStack.closeAll(); //player hud is opened just every other screen.
        openedScreenStack.open(pauseScreen);
        DebugHelper::logMessage("UPlayerUi open pause");
    }
    else
    {
        DebugHelper::logMessage("UPlayerUi try open pause failed, game launch still open.");
    }
}

//called from Pause screen, AworldLevel::gameStateManager flag update needed!
void UPlayerUi::openGameScreen(){

    DebugHelper::logMessage("UPlayerUi open game screen");
    AworldLevel::gameStateManager.OverrideGameState(EGameState::EGamePlay);

    // close ui
    openedScreenStack.closeAll(); //very important
    openedScreenStack.open(playerHud);

    //small test: open player hud instead of manual hid / unhide
    /*if(playerHud){
        playerHud->SetVisible(true);
    }*/

    if(loadoutScreen){
        loadoutScreen->reloadPlayerLoadoutOnUiLeaveIfNeeded();
    }

    
}

void UPlayerUi::openLoadoutScreen(){

    

    if(referenceManager* r = referenceManager::instance()){
        AplayerScript *player = r->getPlayerPointer();
        if(player){
            ALoadoutRoomActor::StaticEnter(player);
            openGameScreen(); //close ui
        }
        return;
    }

    //deprecated
    if(screenSwitchAllowed()){
        //if(playerHud){
        //    playerHud->SetVisible(false);
        //}
        openedScreenStack.closeAll(); //player hud is opened just every other screen.
        openedScreenStack.open(loadoutScreen);
    }
}


void UPlayerUi::openGameLaunchScreen(){
    DebugHelper::logMessage("UPlayerUi open game launch screen");
    if (screenSwitchAllowed())
    {
        //if(playerHud){
        //    playerHud->SetVisible(false);
        //}
        openedScreenStack.open(gameLaunchScreen);
    }
}



bool UPlayerUi::screenSwitchAllowed(){
    return openedScreenStack.ScreenIsOpenTop(gameLaunchScreen) == false;
}





// --- api for hud update ---
UPlayerHud *UPlayerUi::HudInstance(){
    if(playerHud){
        return playerHud;
    }
    return nullptr;
}




bool UPlayerUi::OnCursorClick(){
    if(!Super::OnCursorClick()){


        return false;
    }
    return true;
}