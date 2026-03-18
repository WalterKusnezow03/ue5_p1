#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/throwerWeapon/base/throwableLauncherWeapon.h"

#include "RpgLauncherWeapon.generated.h"

class ARpgThrowable;

UCLASS()
class P2_API ARpgLauncherWeapon : public AthrowableLauncherWeapon {
    GENERATED_BODY()

public:
    ARpgLauncherWeapon();




protected:
    virtual void BeginPlay() override;

    virtual void RequestNewThrowableIfNeeded() override;
    ARpgThrowable *ThrowableCasted();

    void setupReloadAnimationOnBeginPlay();
};
