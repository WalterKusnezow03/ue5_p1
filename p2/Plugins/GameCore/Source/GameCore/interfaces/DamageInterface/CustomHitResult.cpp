#include "CustomHitResult.h"


void FCustomHitResult::SetupHitResult(
    FVector &customHitPoint,
    int inDamage,
    bool inSurpressed,
    float deltaTimeIn
){
    FCustomHitResultBase::SetupHitResult(inDamage, inSurpressed, deltaTimeIn);
    hitPoint = customHitPoint;
    hitPointSetup = true;
    SetDefaultMessageIfNeeded();
}

void FCustomHitResult::SetupHitResult(
    FVector &customHitPoint,
    int inDamage,
    float deltaTimeIn
){
    SetupHitResult(customHitPoint, inDamage, false, deltaTimeIn);
}

void FCustomHitResult::SetupHitResult(
    FHitResult &inResult, 
    int inDamage, 
    bool inSurpressed,
    float deltaTimeIn
){
    actorResult = inResult.GetActor();
    SetupHitResult(inResult.ImpactPoint, inDamage, inSurpressed, deltaTimeIn);
}

void FCustomHitResult::SetupHitResult(
    FHitResult &inResult,
    int inDamage,
    bool inSurpressed,
    FVector &raycastStart,
    float deltaTimeIn
){
    SetupHitResult(inResult, inDamage, inSurpressed, deltaTimeIn);
    UpdateDirectionAndDistance(raycastStart);
}

void FCustomHitResult::UpdateDirectionAndDistance(FVector &startingLocation){
    direction = hitPoint - startingLocation; // AB = B - A
    distance = direction.Size();
    direction = direction.GetSafeNormal();
}


FVector &FCustomHitResult::HitPoint(){
    return hitPoint;
}

const FVector &FCustomHitResult::Direction() const {
    return direction;
}

float FCustomHitResult::Distance() const {
    return distance;
}

bool FCustomHitResult::HasHitPoint() const {
    return hitPointSetup;
}




void FCustomHitResult::SetDefaultMessageIfNeeded(){
    if(message.Len() == 0){
        SetMessage("Default");
    }
}


void FCustomHitResult::LogMessage(FString messagePrefix){
    FString finalMessage = messagePrefix + message;
    DebugHelper::logMessage(finalMessage);
}

void FCustomHitResult::SetMessage(FString s){
    message = TEXT(" FCustomHitResult - ") + s;
}