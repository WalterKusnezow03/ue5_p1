#pragma once

#include "CoreMinimal.h"


class GAMECORE_API FCustomHitResult {

public:
    void SetupHitResult(int damageIn);

    void SetupHitResult(
        int inDamage,
        bool inSurpressed,
        float deltaTimeIn = 0.0f
    );

    void SetupHitResult(
        FHitResult &inResult,
        int inDamage,
        bool inSurpressed,
        float deltaTimeIn = 0.0f
    );

    void SetupHitResult(
        FVector &customHitPoint,
        int inDamage,
        bool inSurpressed,
        float deltaTimeIn = 0.0f
    );

    void SetupHitResult(
        FVector &customHitPoint,
        int inDamage,
        float deltaTimeIn = 0.0f
    );

    void SetupHitResult(
        FHitResult &inResult,
        int inDamage,
        bool inSurpressed,
        FVector &raycastStart, //to find a direction, optionally needed.
        float deltaTimeIn = 0.0f
    );

    int Damage() const;
    bool IsSurpressed() const;
    FVector &HitPoint();
    const FVector &Direction() const;
    float Distance() const;
    bool HasHitPoint() const;
    float DeltaTime() const;

    //debug
    void LogMessage(FString messagePrefix);
    void SetMessage(FString s);

    AActor *GetActor() const;

private:
    int damage = 0;
    
    FVector direction;
    bool surpressed = false;
    FHitResult result;
    float distance = 0.0f;

    float savedDeltaTime = 0.0f;

    FVector hitPoint;
    bool hitPointSetup = false;

    void SetDeltaTime(float deltaTimeIn);

    void UpdateDirectionAndDistance(FVector &startRaycastLocation);
    void SetDefaultMessageIfNeeded();
    FString message;
};