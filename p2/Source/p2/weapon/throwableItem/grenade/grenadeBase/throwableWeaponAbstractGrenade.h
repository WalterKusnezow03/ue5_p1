#pragma once

#include "CoreMinimal.h"


#include "p2/weapon/throwableItem/throwableWeaponBase.h"

#include "GameCore/interfaces/DamageInterface/Damageinterface.h"

#include "throwableWeaponAbstractGrenade.generated.h"

//partially shared logic with explosive damagable base but cant be derived
//--- abstract base class for throwable grenades (shared hand greande and smoke grenade logic) ---
UCLASS()
class P2_API AthrowableWeaponAbstractGrenade : public AthrowableWeaponBase, public IDamageinterface {
    GENERATED_BODY()

public:



    //start the timer for detonation.
    virtual void releaseShoot() override;



protected:

    // --- time delay after throw ---
    UPROPERTY(EditAnywhere, Category="GrenadeProperties")
    float timeDelay = 3.0f;
    bool timerStarted = false;
    bool isDetonated = false;
    Timer delayedExplosionTimer;
    void TickExplosionTimer(float deltatime);
    // --- time delay after throw ---



    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;


    // --- lever ---
    UPROPERTY(EditAnywhere, Category="GrenadeProperties")
    FString leverMeshName;

    UPROPERTY()
    USceneComponent *leverMesh = nullptr;

    UPROPERTY(EditAnywhere, Category="GrenadeProperties")
    float leverReleaseTime = 1.0f;
    Timer leverTimer;

    void FindDetonationLeverOnBeginPlay();
    void TickLeverMovement(float deltatime);
    void TickLeverTimer(float deltatime);
    void ApplyLeverMovement();
    // --- lever ---

    //override if needed (hand grenade and smoke greande different bevaiviour)
    virtual void Detonate();


    //release to object pool
    virtual void release();



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