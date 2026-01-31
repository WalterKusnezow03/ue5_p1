#include "GameStateManager.h"


#include "p2/_world/worldLevel.h"
#include "p2/ui/3Dui/GameStartRoom/GameStartRoom.h"
#include "p2/ui/3Dui/PauseActor/PauseRoomActor.h"

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

void GameStateManager::OverrideGameState(EGameState state){
    currentGameState = state;
}

void GameStateManager::UpdateGameState(EGameState state){
    if(!CanSwitchStates(state)){
        return;
    }
    //LeaveCurrentGameState();
    TryQuitPause(state);

    if (state == EGameState::EGameLaunchScreen){

        AGameStartRoom::StaticEnter(AworldLevel::GetPlayerReference());
        OverrideGameState(state);
        return;
    }

    if(state == EGameState::EPauseScreen){
        APauseRoomActor::StaticEnter(AworldLevel::GetPlayerReference());
        OverrideGameState(state);
        return;
    }



    OverrideGameState(state);


    /*
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
    }*/
}

void GameStateManager::TryQuitPause(EGameState state){
    if(
        IsInState(EGameState::EPauseScreen) &&
        state == EGameState::EGamePlay
    ){
        APauseRoomActor::StaticLeave();
        OverrideGameState(state);
    }
}



/*
void GameStateManager::LeaveCurrentGameState(){
    if(currentGameState == EGameState::ESetupRoom){

    }
}
*/

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
    return IsInState(EGameState::EPauseScreen);
    
    //return !IsInState(EGameState::EGamePlay);
    
    // IsInState(EGameState::EPauseScreen) ||
    // IsInState(EGameState::EGameLaunchScreen);
}