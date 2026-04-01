#include "WeaponPropertyAsset.h"
#include "DebugPlugin/DebugHelper.h"



bool FWeaponPropertyAsset::GetUiName(FString &outName){
    //DebugHelper::logMessage("UWeaponEnumAssetPack::toString FWeaponPropertyAsset ", uiWeaponName);
    if (uiWeaponName.Len() > 0)
    {
        outName = uiWeaponName;
        return true;
    }
    return false;
}

bool FWeaponPropertyAsset::IsThrowable(){
    return isAThrowable;
}

bool FWeaponPropertyAsset::IsSameAmmunition(ammunitionEnum compare){
    return typeAmmunition == compare;
}


bool FWeaponPropertyAsset::IsSameAmmunitionAndThrowable(ammunitionEnum compare){
    return IsSameAmmunition(compare) && IsThrowable();
}





/// ---- weapon properties dynamic api init ----
void FWeaponPropertyAsset::ValidatePropertiesOnInit(){
    setupMaxDamage(maxDamage);
    setupRange(maxRange);
    setupFireRateRPM(fireRpm);
    setupMagSize(magSize);
    setupReloadTime(reloadTimeOfWeapon);
    setupRecoilValue(recoilValueSaved);
}

void FWeaponPropertyAsset::setupRange(int rangeInCm){
    rangeInCm = std::abs(rangeInCm);
    maxRangeCMSquared = std::abs(rangeInCm) * std::abs(rangeInCm);
    maxRangeCMSquared = std::max(1.0f, maxRangeCMSquared);
}

void FWeaponPropertyAsset::setupMaxDamage(int damageIn){
    maxDamage = damageIn; //must not be negative, could be healing.
}

void FWeaponPropertyAsset::setupFireRateRPM(float fireRateIn){
    fireRpm = std::abs(fireRateIn);
    fireRpm = std::max(fireRpm, 1.0f);
    findCoolDownTimeBasedOnRpm();
}


/// ---- internal private ----
/*
 * calculates time to wait for a int of rounds per minute
 */
/// @brief caution: fireRpm should be resonably high to avoind div by zero
void FWeaponPropertyAsset::findCoolDownTimeBasedOnRpm(){
    if(fireRpm <= 0.000000001f){
        animationTimeBasedOnRpm = 1.0f;
        return;
    }

    float roundsPerSecond = fireRpm / 60.0f;
    roundsPerSecond = std::max(roundsPerSecond, 0.000000001f); //some random value
    animationTimeBasedOnRpm = 1.0f / roundsPerSecond; //1s / rps = wait time in seconds
}


void FWeaponPropertyAsset::setupMagSize(int invalue){
    magSize = std::abs(invalue);
    magSize = std::max(magSize, 1);
}

void FWeaponPropertyAsset::setupReloadTime(float timein){
    reloadTimeOfWeapon = std::abs(timein);
}

void FWeaponPropertyAsset::setupRecoilValue(float valueIn){
    recoilValueSaved = std::abs(valueIn) * -1.0f; //up pitch negative for. reasons. Which shouldnt be.
}




/// ---- weapon properties dynamic api runtime ----

/// @brief damage over a distance by weapon type
/// @param start start point
/// @param target target hit point
float FWeaponPropertyAsset::damageFor(FVector &start, FVector &target) const {
    //linearer abfall vom damage

    //skalar = distTarget / distAll
    //skalarInverted = 1.0 - skalar
    //damage = damageMax * skalarInverted
    //min(1, damage)

    float distSquaredCm = FVector::DistSquared(start, target);
    float skalar = distSquaredCm / maxRangeCMSquared;
    float skalarInverted = 1.0f - skalar;
    float damageOut = maxDamage * skalarInverted;

    //min 1 either direction
    float sign = maxDamage < 0.0f ? -1.0f : 1.0f;
    if(std::abs(damageOut) < 0.0f){
        damageOut = 1.0f * sign;
    }
    int asInt = damageOut;
    DebugHelper::logMessage("FWeaponPropertyAsset::DAMAGE ", asInt);
    return damageOut;
}


int FWeaponPropertyAsset::getMagSize() const {
    return magSize;
}

float FWeaponPropertyAsset::recoilValue() const {
    return recoilValueSaved;
}

float FWeaponPropertyAsset::CooldownTimeBasedOnRpm() const {
    return animationTimeBasedOnRpm;
}

float FWeaponPropertyAsset::reloadTime() const {
    return reloadTimeOfWeapon;
}

bool FWeaponPropertyAsset::isSingleFireOnly() const {
    return isSingleFire;
}

/// @brief used for player inventory reloading
/// @return type
ammunitionEnum FWeaponPropertyAsset::getAmmunitionType() const {
    return typeAmmunition;
}
