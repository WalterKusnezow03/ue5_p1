#pragma once

#include "CoreMinimal.h"

class GAMECORE_API FCustomHitResultBase{

public:
    void SetupHitResult(int damageIn);

    void SetupHitResult(
        int inDamage,
        bool inSurpressed,
        float deltaTimeIn = 0.0f
    );

    void SetupHitResult(
        int inDamage,
        bool inSurpressed,
        float deltaTimeIn,
        AActor *actor
    );

    void SetupHitResult(
        int inDamage,
        bool inSurpressed,
        float deltaTimeIn,
        AActor *actor,
        const FVector &directionIn
    );

    
    int Damage() const;
    bool IsSurpressed() const;
    float DeltaTime() const;
    AActor *GetActor() const;

protected:
    int damage = 0;
    bool surpressed = false;
    float savedDeltaTime = 0.0f;
    AActor *actorResult = nullptr;
    FVector direction;

    void SetDeltaTime(float deltaTimeIn);

};