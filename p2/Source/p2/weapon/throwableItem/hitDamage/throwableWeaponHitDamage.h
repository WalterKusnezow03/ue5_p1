#pragma once

#include "p2/weapon/throwableItem/throwableWeaponBase.h"
#include "CoreMath/util/SphereCaster.h"

#include "throwableWeaponHitDamage.generated.h"

UCLASS()
class P2_API AthrowableWeaponHitDamage : public AthrowableWeaponBase {
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, Category="throwableProperty")
    int throwableDamage = 25;

protected:
    
    
    virtual void Tick(float deltatime) override;
    void TickCheckHit(float DeltaTime);

    void ProcessHit(
        TArray<AActor *> &castedActors,
        float DeltaTime
    );

    SphereCaster sphereCaster;

    FCustomHitResult MakeHitResultByFoundActor(
        AActor *hitActor,
        float DeltaTime
    );
};