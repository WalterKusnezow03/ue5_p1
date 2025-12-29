#pragma once

#include "CoreMinimal.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/weapon/ammunitionEnum.h"

/// @brief will manage damage, damage by distance and recoil for a weapon
class P2_API WeaponProperties{

public:
    WeaponProperties();
    ~WeaponProperties();

    // ---- setup api ----
    void setup(
        weaponEnum weaponTypeIn, 
        ammunitionEnum ammunitionIn,
        int maxRangeCm, 
        int maxDamageIn,
        float fireRpmIn,
        bool isSingleFireIn,
        int magSize,
        float reloadTimeIn,
        float recoilUp
    );
    
    void setupRange(int rangeInCm);
    void setupMaxDamage(int damageIn);
    void setupFireRateRPM(float fireRate);
    void setSingleFire(bool isSingleFireIn);
    void setupMagSize(int magSizeIn);
    void setupReloadTime(float reloadTimeIn);
    void setupRecoilValue(float valueIn);

    // ---- dynamic api - const! ----
    int getMagSize() const;
    float recoilValue() const;
    float reloadTime() const;

    /// @brief damage over a distance by weapon type
    /// @param a 
    /// @param b 
    float damageFor(FVector &a, FVector &b) const;
    float CooldownTimeBasedOnRpm() const;
    ammunitionEnum getAmmunitionType() const;
    bool isSingleFireOnly() const;

private:
    //damage
    float maxDamage = 10;
    float maxRangeCMSquared = 100.0f;

    //mag
    int magSize = 10;
    float reloadTimeSaved = 1.5f;

    weaponEnum weaponTypeSaved;
    ammunitionEnum ammunitionTypeSaved;

    //fire mode
    float fireRpm = 300;
    float animationTimeBasedOnRpm = 0.1f;
    bool isSingleFire = false;

    float recoilValueSaved = -0.1f; //pitch up

    //available sight, grip, muzzle attachments here (?)

    void findCoolDownTimeBasedOnRpm();
};
