#include "WeaponProperties.h"


WeaponProperties::WeaponProperties(){

}

WeaponProperties::~WeaponProperties(){

}

/// @brief setup weapon properties for calculation of damage
/// @param weaponTypeIn 
/// @param maxRangeCm 
/// @param maxDamageIn 
void WeaponProperties::setup(
    weaponEnum weaponTypeIn, 
    ammunitionEnum ammoTypeIn,
    int maxRangeCm, 
    int maxDamageIn,
    float fireRpmIn,
    bool isSingleFireIn,
    int magSizeIn,
    float reloadTimeIn,
    float recoilUp
){
    ammunitionTypeSaved = ammoTypeIn;
    weaponTypeSaved = weaponTypeIn; // not needed basically
    setSingleFire(isSingleFireIn);
    setupRange(maxRangeCm);
    setupMaxDamage(maxDamageIn);
    setupFireRateRPM(fireRpmIn);
    setupMagSize(magSizeIn);
    setupReloadTime(reloadTimeIn);
    setupRecoilValue(recoilUp);
}

/// @brief sets up max range of weapon
/// @param rangeInCm 
void WeaponProperties::setupRange(int rangeInCm){
    rangeInCm = std::abs(rangeInCm);
    maxRangeCMSquared = std::abs(rangeInCm) * std::abs(rangeInCm);
    maxRangeCMSquared = std::max(1.0f, maxRangeCMSquared);
}

void WeaponProperties::setupMaxDamage(int damageIn){
    maxDamage = damageIn; //must not be negative, could be healing.
}

void WeaponProperties::setupFireRateRPM(float fireRateIn){
    fireRpm = std::abs(fireRateIn);
    fireRpm = std::max(fireRpm, 1.0f);
    findCoolDownTimeBasedOnRpm();
}

void WeaponProperties::setSingleFire(bool bIn){
    isSingleFire = bIn;
}

void WeaponProperties::setupMagSize(int invalue){
    magSize = std::abs(invalue);
    magSize = std::max(magSize, 1);
}

void WeaponProperties::setupReloadTime(float timein){
    reloadTimeSaved = std::abs(timein);
}

void WeaponProperties::setupRecoilValue(float valueIn){
    recoilValueSaved = std::abs(valueIn) * -1.0f; //up pitch negative for. reasons. Which shouldnt be.
}

/// dynamic api

/// @brief damage over a distance by weapon type
/// @param start start point
/// @param target target hit point
float WeaponProperties::damageFor(FVector &start, FVector &target) const {
    //linearer abfall vom damage

    //skalar = distTarget / distAll
    //skalarInverted = 1.0 - skalar
    //damage = damageMax * skalarInverted
    //min(1, damage)

    float distSquaredCm = FVector::DistSquared(start, target);
    float skalar = distSquaredCm / maxRangeCMSquared;
    float skalarInverted = 1.0f - skalar;
    float damageOut = maxDamage * skalarInverted;

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("WeaponProperties: Damage (DistSquared: %.2f) (MaxDistance: %.2f) (skalar Inv: %2f), (damage: %.2f)"),
            distSquaredCm,
            maxRangeCMSquared,
            skalarInverted,
            damageOut
        )
    );

    //min 1 either direction
    float sign = maxDamage < 0.0f ? -1.0f : 1.0f;
    if(std::abs(damageOut) < 0.0f){
        damageOut = 1.0f * sign;
    }
    return damageOut;
}


int WeaponProperties::getMagSize() const {
    return magSize;
}

float WeaponProperties::recoilValue() const {
    return recoilValueSaved;
}

float WeaponProperties::CooldownTimeBasedOnRpm() const {
    return animationTimeBasedOnRpm;
}

float WeaponProperties::reloadTime() const {
    return reloadTimeSaved;
}

bool WeaponProperties::isSingleFireOnly() const {
    return isSingleFire;
}

/// @brief used for player inventory reloading
/// @return type
ammunitionEnum WeaponProperties::getAmmunitionType() const {
    return ammunitionTypeSaved;
}


/// ---- internal private ----
/**
 * calculates time to wait for a int of rounds per minute
 */

/// @brief caution: fireRpm should be resonably high to avoind div by zero
void WeaponProperties::findCoolDownTimeBasedOnRpm(){
	float roundsPerSecond = fireRpm / 60.0f;
    roundsPerSecond = std::max(roundsPerSecond, 0.000000001f); //some random value

    animationTimeBasedOnRpm = 1.0f / roundsPerSecond; //1s / rps = wait time in seconds
}

