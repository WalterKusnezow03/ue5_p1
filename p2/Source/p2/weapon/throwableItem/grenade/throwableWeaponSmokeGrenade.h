#pragma once

#include "CoreMinimal.h"

#include "p2/weapon/ExplosiveHelper/ExplosiveHelper.h"
#include "p2/weapon/throwableItem/grenade/grenadeBase/throwableWeaponAbstractGrenade.h"

#include "throwableWeaponSmokeGrenade.generated.h"

UCLASS()
class P2_API AthrowableWeaponSmokeGrenade : public AthrowableWeaponAbstractGrenade {
    GENERATED_BODY()


public:

    //virtual void takedamage(FCustomHitResult &result) override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    virtual void Detonate() override;
    virtual void release() override;

    void FindNiagaraSmokeOnBeginPlay();
    void TickNiagaraComponent(float deltatime);

    UPROPERTY(EditAnywhere, Category="SmokeGrenadeProperties")
    FString smokeNiagaraComponentName;

    UPROPERTY()
    class UNiagaraComponentCustom *niagaraSmoke = nullptr;

    // --- life time tick (stop smoke a little before) ---
    UPROPERTY(EditAnywhere, Category="SmokeGrenadeProperties")
    float maxLifeTime = 60.0f;
    Timer maxLifeTimeTimer;
    void TickLifeTime(float deltatime);
    
};