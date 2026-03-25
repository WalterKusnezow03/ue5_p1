#include "PlayerStatusManager.h"

#include "p2/ui/3Dui/HUD/HudUiActor.h"
#include "p2/player/inventory/PlayerInventory.h"

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
    
    //new 3D ui
    if(AHudUiActor *actor = AHudUiActor::GetInstance()){
        if(WasModified()){
            actor->Update(GetFPlayerStatus());
        }
        return;
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


void PlayerStatusManager::updateInventory(PlayerInventory &inventory){
    if(playerStatusData.UpdatePlayerInventory(inventory)){
        FlagModifiedTrue();
        UpdatePlayerHud();
    }
}