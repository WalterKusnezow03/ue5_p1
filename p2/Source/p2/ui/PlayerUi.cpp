// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUi.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "p2/ui/alignmentPresets/PresetCornersLayout.h"
#include "p2/ui/screens/PlayerHud.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/ui/Widgets/TextBased/TextAndImage.h"
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
    if(loadoutScreen){
        loadoutScreen->init(*this);
        loadoutScreen->setVisible(false);
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




//player interact api

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







/// ----- MANUAL CLICK DISPATCHER ------

/// @brief manually registers clicks for UMG UI! Needed to be updated every frame!
void UPlayerUi::updateClickDispatch(){

    //click dispatch!
    TSet<FKey> PressedButtons = FSlateApplication::Get().GetPressedMouseButtons();
    if (PressedButtons.Contains(EKeys::LeftMouseButton)) {
        // Linksklick ist aktuell gedrückt
        RegisterCursorClick();
        DebugHelper::showScreenMessage("SLATE CLICK REGISTERED", FColor::Red);
        return;
    }

    //Hover dispatch
}

// new manual cursor dispatching clicks!
void UPlayerUi::RegisterCursorClick(){
    openedScreenStack.dispatchClick();
}