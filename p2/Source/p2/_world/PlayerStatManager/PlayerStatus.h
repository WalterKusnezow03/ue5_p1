#pragma once

#include "CoreMinimal.h"

/// @brief struct storing player info for HUD
struct P2_API FPlayerStatus {

    FPlayerStatus(){};
    ~FPlayerStatus(){};

    int health = 0;
    int maxHealth = 0;

    int ammo = 0;
    int maxAmmo = 0;

    int mags = 0;



    ///@brief update and return if anything changed
    bool UpdateHealth(int number);
    bool UpdateHealthMax(int number);
    bool UpdateAmmunition(int number);
    bool UpdateAmmunitionMax(int number);



    //api for HUD
    FString HealthString();
    FString AmmunitionString();

    /// @brief value between [0,1] if maxhealth is a valid number.
    float healthAsScalar();
};