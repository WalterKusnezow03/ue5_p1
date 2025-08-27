#include "GameStateManager.h"
#include "p2/ui/PlayerUi.h"

GameStateManager::GameStateManager(){

}

GameStateManager::~GameStateManager(){

}

void GameStateManager::UpdateGameState(EGameState state){
    if(state == EGameState::EGameLaunchScreen){
        UPlayerUi *instance = UPlayerUi::currentInstance();
        if(instance){
            instance->openGameLaunchScreen();
        }
    }
}