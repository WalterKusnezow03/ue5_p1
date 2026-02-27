#pragma once

#include "CoreMinimal.h"


class GAMECORE_API FCustomHitResult {

public:
    void SetupHitResult(int damageIn);

    void SetupHitResult(
        int inDamage,
        bool inSurpressed
    );

    void SetupHitResult(
        FHitResult &inResult,
        int inDamage,
        bool inSurpressed
    );

    void SetupHitResult(
        FVector &customHitPoint,
        int inDamage,
        bool inSurpressed
    );

    void SetupHitResult(
        FVector &customHitPoint,
        int inDamage
    );

    void SetupHitResult(
        FHitResult &inResult,
        int inDamage,
        bool inSurpressed,
        FVector &raycastStart //to find a direction, optionally needed.
    );

    int Damage();
    bool IsSurpressed();
    FVector &HitPoint();
    FVector &Direction();
    float Distance();
    bool HasHitPoint();

private:
    int damage = 0;
    
    FVector direction;
    bool surpressed = false;
    FHitResult result;
    float distance = 0.0f;

    FVector hitPoint;
    bool hitPointSetup = false;

    void UpdateDirectionAndDistance(FVector &startRaycastLocation);

};