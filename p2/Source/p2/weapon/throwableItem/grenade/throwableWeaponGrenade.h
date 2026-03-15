#pragma once

#include "CoreMinimal.h"

#include "p2/weapon/ExplosiveHelper/ExplosiveHelper.h"
#include "p2/weapon/throwableItem/throwableWeaponBase.h"

#include "GameCore/interfaces/DamageInterface/Damageinterface.h"

#include "throwableWeaponGrenade.generated.h"

//partially shared logic with explosive damagable base but cant be derived
UCLASS()
class P2_API AthrowableWeaponGrenade : public AthrowableWeaponBase, public IDamageinterface {
    GENERATED_BODY()

public:



    //start the timer for detonation.
    virtual void releaseShoot() override;



protected:
    UPROPERTY(EditAnywhere, Category="GrenadeProperties")
    float timeDelay = 5.0f;

    bool timerStarted = false;
    bool isDetonated = false;
    Timer delayedExplosionTimer;

    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    void TickExplosionTimer(float deltatime);



    //override if needed
    virtual void Detonate();


    //change damage / radius vars on derived beginplay to override
    void SetupExplosiveHelperOnBeginPlay();

    //release to object pool
    void release();

    ExplosiveHelper explosiveHelper;

    int EXPLOSION_RADIUS = 10000; //20 000 --> (20 * 100 = 20m)
	int DAMAGE = 40;
	int DAMAGE_RADIUS = 5000;


    void ResetDenonatedStatus();


public:
    //damage interface derived
	virtual void takedamage(FCustomHitResult &result) override;

    virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

    teamEnum team = teamEnum::neutralTeam;

    virtual bool IsInRange(const FVector &position, float maxDistance) override;

protected:
    USceneComponent *FindHandCarriedScene(EArmType type) override;
};