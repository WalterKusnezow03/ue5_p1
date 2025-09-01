#pragma once

#include "CoreMinimal.h"

#include "p2/_world/PlayerStatManager/PlayerStatus.h"

class P2_API PlayerStatusManager {

public:
    PlayerStatusManager();
    ~PlayerStatusManager();

    //-- player API --
    void updateAmmunition(int ammoCurrentIn, int ammoMaxIn);
    void updateHealth(int health);
    void updateHealthMax(int health);

    //-- UI API --
    
    ///@brief returns the modified flag and resets it!
    bool WasModified();

    ///@brief player status struct, not modifiable
    FPlayerStatus &GetFPlayerStatus();

private:
    bool bModifiedFlag = true;
    void FlagModifiedTrue();

    /// @brief propagates the changes, if made to the player hud
    void UpdatePlayerHud();

    FPlayerStatus playerStatusData;
};