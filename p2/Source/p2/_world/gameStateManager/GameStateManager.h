#pragma once 

#include "CoreMinimal.h"
#include "p2/_world/EGameState.h"

class P2_API GameStateManager {

public:
    GameStateManager();
    ~GameStateManager();

    /// @brief will set the game state and open certain ui if needed,
    /// ui must be initialized before!
    /// @param state 
    void UpdateGameState(EGameState state);

private:
    EGameState currentGameState = EGameState::EGameLaunchScreen;
};