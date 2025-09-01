#include "PlayerStatus.h"


FString FPlayerStatus::HealthString(){
    FString outString;
    outString += FString::Printf(TEXT("%d"), health);
    if(maxHealth >= 0){
        outString += FString::Printf(TEXT("|%d"), maxHealth);
    }
    return outString;
}

FString FPlayerStatus::AmmunitionString(){
    FString outString;
    outString += FString::Printf(TEXT("%d"), ammo);
    if(maxAmmo > 0){
        outString += FString::Printf(TEXT("|%d"), maxAmmo);
    }
    return outString;
}



bool FPlayerStatus::UpdateHealth(int number){
    bool changed = number != health;
    health = number;
    return changed;
}

bool FPlayerStatus::UpdateHealthMax(int number){
    bool changed = number != maxHealth;
    maxHealth = number;
    return changed;
}

float FPlayerStatus::healthAsScalar(){
    if(maxHealth != 0){
        float denominator = maxHealth;
        float nominator = health;
        return nominator / denominator;
    }
    return 1.0f;
}

bool FPlayerStatus::UpdateAmmunition(int number){
    bool changed = number != ammo;
    ammo = number;
    return changed;
}

bool FPlayerStatus::UpdateAmmunitionMax(int number){
    bool changed = number != maxAmmo;
    maxAmmo = number;
    return changed;
}