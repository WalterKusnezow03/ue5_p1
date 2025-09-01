#include "PlayerStatusManager.h"
#include "p2/ui/PlayerUi.h"

PlayerStatusManager::PlayerStatusManager(){

}



PlayerStatusManager::~PlayerStatusManager(){

}


bool PlayerStatusManager::WasModified(){
    bool copy = bModifiedFlag;
    bModifiedFlag = false;
    return copy;
}

void PlayerStatusManager::FlagModifiedTrue(){
    bModifiedFlag = true;
}

void PlayerStatusManager::UpdatePlayerHud(){
    UPlayerUi *ui = UPlayerUi::currentInstance();
    if(ui){
        UPlayerHud *hud = ui->HudInstance();
        if(hud != nullptr && WasModified()){
            hud->Update(GetFPlayerStatus());
        }
    }
}

FPlayerStatus &PlayerStatusManager::GetFPlayerStatus(){
    return playerStatusData;
}


void PlayerStatusManager::updateAmmunition(int ammoCurrentIn, int ammoMaxIn){
    if(
        playerStatusData.UpdateAmmunition(ammoCurrentIn) ||
        playerStatusData.UpdateAmmunitionMax(ammoMaxIn)
    ){
        FlagModifiedTrue();
        UpdatePlayerHud();
    }
}

void PlayerStatusManager::updateHealth(int number){
    if(playerStatusData.UpdateHealth(number)){
        FlagModifiedTrue();
        UpdatePlayerHud();
    }
}


void PlayerStatusManager::updateHealthMax(int number){
    if(playerStatusData.UpdateHealthMax(number)){
        FlagModifiedTrue();
        UpdatePlayerHud();
    }
}