// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "p2/gameStart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "p2/gameStart/assetEnums/textureEnum.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "p2/ui/screens/PlayerHud.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/ui/makeUWidgets/TextAndImage.h"
#include "p2/ui/screens/loadout/LoadoutScreen.h"

//instance maker with init call!
UPlayerUi* UPlayerUi::createNewInstance(UWorld *world){
    if(world != nullptr){
        assetManager *pointer = assetManager::instance();
        if(pointer != nullptr){
            UClass *blueprint = pointer->uiBp();

            if(blueprint != nullptr){

                UPlayerUi* newUiInstance = CreateWidget<UPlayerUi>(world, blueprint);
                if (newUiInstance != nullptr)
                {
                    newUiInstance->init();
                    return newUiInstance;
                }
            }
        }
    }
    return nullptr;
}


///@brief do not delete, only for ui components!
UCanvasPanel *UPlayerUi::canvasPanelPointer(){
    return baseCanvas;
}

//constructor like
void UPlayerUi::init(){
    if(isInited){
        return;
    }
    isInited = true;


    //the root widget must be a canvas panel in bp!
    findBaseCanvasFromBluePrint();

    createBasePlayerHud();
    createPauseScreen();
    createLoadoutScreen();
}

void UPlayerUi::findBaseCanvasFromBluePrint(){
    UWidget* RootWidget = GetRootWidget();
    if (RootWidget)
    {
        UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(RootWidget);
        if (CanvasPanel)
        {
            baseCanvas = CanvasPanel;
        }
    }
}


/// ----- add to own ------

///@brief will add any widget to the base canvas, if not nullptr!
void UPlayerUi::addToCanvas(UWidget *any){
    if(any != nullptr && baseCanvas != nullptr){
        baseCanvas->AddChild(any);
    }
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
    loadoutScreen->init(*this);

    loadoutScreen->setVisible(false);
}



//public api

void UPlayerUi::updateAmmunitionText(int number){
    playerHud->updateAmmunitionText(number);
}

void UPlayerUi::updateAmmunitionText(FString message){
    playerHud->updateAmmunitionText(message);
}

void UPlayerUi::updateHealthText(int health){
    playerHud->updateHealthText(health);
}






void UPlayerUi::openPauseScreen(){
    playerHud->setVisible(false);
    openedScreenStack.open(pauseScreen);
    showPlayerCursor(true);
}

void UPlayerUi::openGameScreen(){
    openedScreenStack.closeAll();
    playerHud->setVisible(true);
    showPlayerCursor(false);

    if(loadoutScreen){
        loadoutScreen->reloadPlayerLoadoutOnUiLeaveIfNeeded();
    }

    //the exit button doesnt do that on its own. Setting the flag to false
    //is okay.
    referenceManager::forceSetPlayerPaused(false);
}

void UPlayerUi::openLoadoutScreen(){
    playerHud->setVisible(false);
    openedScreenStack.open(loadoutScreen);
}

void UPlayerUi::closeLatestScreen(){
    DebugHelper::logMessage("debugClose screen");
    openedScreenStack.closeBack();
    if(openedScreenStack.isEmpty()){
        openGameScreen();
    }
}

void UPlayerUi::showPlayerCursor(bool show){
    referenceManager::showPlayerCursor(show);
}