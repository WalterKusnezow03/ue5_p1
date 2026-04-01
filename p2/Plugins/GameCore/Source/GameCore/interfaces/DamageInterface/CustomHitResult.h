#pragma once

#include "CoreMinimal.h"
#include "GameCore/interfaces/DamageInterface/CustomHitResultBase/CustomHitResultBase.h"


class GAMECORE_API FCustomHitResult : public FCustomHitResultBase{

public:
    
    using FCustomHitResultBase::SetupHitResult;
    
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

    bool HasHitPoint() const ;
    FVector &HitPoint();
    const FVector &Direction() const;
    float Distance() const;
    

    //debug
    void LogMessage(FString messagePrefix);
    void SetMessage(FString s);

    

private:
    
    
    float distance = 0.0f;

    FVector hitPoint;
    bool hitPointSetup = false;

    void UpdateDirectionAndDistance(FVector &startRaycastLocation);
    void SetDefaultMessageIfNeeded();
    FString message;
};