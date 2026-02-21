#pragma once

#include "CoreMinimal.h"

/// @brief will track an aactor and flag for eject on animation finish
/// designed for FArmAnimation pair: eject actor on finish.
/// Designed to apply impulse asewell.
class IKHUMANOIDMODELL_API FAnimationAttachedActor {

public:
    FAnimationAttachedActor();
    ~FAnimationAttachedActor();

    FAnimationAttachedActor(const FAnimationAttachedActor &other);
    FAnimationAttachedActor &operator=(const FAnimationAttachedActor &other);

    void SetActorReference(AActor *actor, bool ejectOnAnimationFinish);
    void ResetActorReference();

    void Eject(FVector &location, FVector &direction, float velocity, float mass);
    void UpdateActorLocation(FVector &location);

private:
    bool ejectOnAnimationFinishFlag = false;
    AActor *actorReference = nullptr;

    UPrimitiveComponent *ActorRoot();
};

