#pragma once


#include "p2/weapon/ExplosiveHelper/ExplosiveHelper.h"
#include "p2/throwableItems/base/LaunchableItemBase.h"

#include "CoreMath/animation/timer/Timer.h"

#include "RpgThrowable.generated.h"

UCLASS()
class P2_API ARpgThrowable : public ALaunchableItemBase {
    GENERATED_BODY()

public:
    ARpgThrowable();

    virtual void reset() override;
    void AddIgnoredActor(AActor *actor);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "rpgProperty")
    float minDetonationTimer = 0.5f;

    virtual void throwIntoDirection(FVector start, FVector direction) override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    void StartTimer();
    bool TickTimer(float deltatime);

    virtual void release() override;

    //void AddImpulseUsingLatestThrowingDirection(float veloctiy);

    void UpdateVelocity(float deltatime);
    void UpdateSphereCast();
    void Detonate();

    void resetIgnoredActors();
    TArray<AActor *> ignoredActors;

    ExplosiveHelper explosiveHelper;
    bool isDetonated = false;

    Timer detonationTimer;
    bool timerFinished = true;

    bool hitObject = false;

    float sphereCastRadius = 50.0f;
};