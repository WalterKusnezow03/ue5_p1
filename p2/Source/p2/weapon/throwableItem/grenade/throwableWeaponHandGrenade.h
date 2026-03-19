#pragma once

#include "CoreMinimal.h"

#include "p2/weapon/ExplosiveHelper/ExplosiveHelper.h"
#include "p2/weapon/throwableItem/grenade/grenadeBase/throwableWeaponAbstractGrenade.h"

#include "throwableWeaponHandGrenade.generated.h"

UCLASS()
class P2_API AthrowableWeaponHandGrenade : public AthrowableWeaponAbstractGrenade {
    GENERATED_BODY()


public:

    virtual void takedamage(FCustomHitResult &result) override;

protected:
    virtual void BeginPlay() override;
    virtual void Detonate() override;

    //change damage / radius vars on derived beginplay to override
    void SetupExplosiveHelperOnBeginPlay();

    ExplosiveHelper explosiveHelper;

    int EXPLOSION_RADIUS = 10000; //20 000 --> (20 * 100 = 20m)
	int DAMAGE = 40;
	int DAMAGE_RADIUS = 5000;



};