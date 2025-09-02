#include "GameStateManager.h"
#include "p2/ui/PlayerUi.h"

GameStateManager::GameStateManager(){

}

GameStateManager::~GameStateManager(){

}

bool GameStateManager::CanSwitchStates(EGameState stateIn){
    if(currentGameState == EGameState::EGameLaunchScreen){
        return stateIn == EGameState::EGamePlay;
    }
    return true;
}

void GameStateManager::UpdateGameState(EGameState state){
    if(!CanSwitchStates(state)){
        return;
    }
    currentGameState = state;
    UPlayerUi *instance = UPlayerUi::currentInstance();
    if(instance){
        if(state == EGameState::EGameLaunchScreen){    
            instance->openGameLaunchScreen();
        }
        if(state == EGameState::EPauseScreen){
            instance->openPauseScreen();
        }
        if(state == EGameState::EGamePlay){
            instance->openGameScreen();
        }
    }
}

void GameStateManager::SetGameStatePaused(){
    UpdateGameState(EGameState::EPauseScreen);
}


void GameStateManager::SwitchGameStatePausedAndChangeUi(){
    //only switchable if in play or pause
    if(IsInState(EGameState::EGamePlay)){
        SetGameStatePaused();
        return;
    }
    if(IsInState(EGameState::EPauseScreen)){
        UpdateGameState(EGameState::EGamePlay);
        return;
    }
}

bool GameStateManager::IsInState(EGameState stateIn){
    return currentGameState == stateIn;
}


bool GameStateManager::GameStateIsPaused(){
    return IsInState(EGameState::EPauseScreen) ||
           IsInState(EGameState::EGameLaunchScreen);
}