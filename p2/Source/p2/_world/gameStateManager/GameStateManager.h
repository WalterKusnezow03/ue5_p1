#pragma once 

#include "CoreMinimal.h"
#include "p2/_world/EGameState.h"

/// @brief reacts to game state changes and propagates to UPlayerUi Instance
class P2_API GameStateManager {

public:
    GameStateManager();
    ~GameStateManager();

    /// @brief will set the game state and open certain ui if needed,
    /// ui must be initialized before!
    /// @param state 
    void UpdateGameState(EGameState state);

    /// @brief sets the game state if allowed,
    ///opens the ui
    void SetGameStatePaused();

    /// @brief switches to pause or closes pause screen, only if currently in play or pause mode!
    void SwitchGameStatePausedAndChangeUi();

    ///@brief tells if currently paused by player (pause, loadscreen, all of them)
    bool GameStateIsPaused();

private:
    bool CanSwitchStates(EGameState stateIn);
    bool IsInState(EGameState stateIn);

    EGameState currentGameState = EGameState::EGamePlay;
};