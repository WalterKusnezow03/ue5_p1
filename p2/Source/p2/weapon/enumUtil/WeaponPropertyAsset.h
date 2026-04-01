#pragma once

#include "CoreMinimal.h"
#include "WeaponPropertyAsset.generated.h"

/// @brief struct to save weapon properties
/// (conversion of weapon enum to string, damage, range, attachments available, ammunition)
USTRUCT(BlueprintType)
struct FWeaponPropertyAsset
{
    GENERATED_BODY()

public:
    void ValidatePropertiesOnInit();

    FWeaponPropertyAsset() {};

    UPROPERTY(EditAnywhere, Category="WeaponConversionData")
    FString uiWeaponName;

    UPROPERTY(EditAnywhere, Category="WeaponConversionData")
    bool CanHaveAttachments = true;

    UPROPERTY(EditAnywhere, Category="WeaponConversionData")
    bool isAThrowable = false;

    UPROPERTY(EditAnywhere, Category="WeaponConversionData")
    ammunitionEnum typeAmmunition = ammunitionEnum::assaultrifle556;

    bool IsSameAmmunitionAndThrowable(ammunitionEnum compare);
    bool IsThrowable();
    bool IsSameAmmunition(ammunitionEnum compare);


    //returns true if name valid
    bool GetUiName(FString &outName);

protected:
    UPROPERTY(EditAnywhere, Category="WeaponPropertyData")
    float maxDamage = 10.0f;

    UPROPERTY(EditAnywhere, Category="WeaponPropertyData")
    float maxRange = 10000.0f; //100 meters

    UPROPERTY(EditAnywhere, Category="WeaponPropertyData")
    float fireRpm = 300;

    UPROPERTY(EditAnywhere, Category="WeaponPropertyData")
    bool isSingleFire = false;

    UPROPERTY(EditAnywhere, Category="WeaponPropertyData")
    int magSize = 10;

    UPROPERTY(EditAnywhere, Category="WeaponPropertyData")
    int reloadTimeOfWeapon = 1;

public:
    // ---- wepaon dynamic api - const! ----
    int getMagSize() const;
    float recoilValue() const;
    float reloadTime() const;

    /// @brief damage over a distance by weapon type
    float damageFor(FVector &a, FVector &b) const;
    float CooldownTimeBasedOnRpm() const;
    ammunitionEnum getAmmunitionType() const;
    bool isSingleFireOnly() const;

private:
    //damage
    float maxRangeCMSquared = 100.0f;

    //fire mode
    float animationTimeBasedOnRpm = 0.1f;
    float recoilValueSaved = -0.1f; //pitch up
    void findCoolDownTimeBasedOnRpm();

    //setup validation
    void setupRange(int rangeInCm);
    void setupMaxDamage(int damageIn);
    void setupFireRateRPM(float fireRate);
    void setSingleFire(bool isSingleFireIn);
    void setupMagSize(int magSizeIn);
    void setupReloadTime(float reloadTimeIn);
    void setupRecoilValue(float valueIn);
};