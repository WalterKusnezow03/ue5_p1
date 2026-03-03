#include "CustomHitResult.h"

void FCustomHitResult::SetupHitResult(int damageIn){
    SetupHitResult(damageIn, false);
}

void FCustomHitResult::SetupHitResult(
    int inDamage,
    bool inSurpressed,
    float deltaTimeIn
){
    damage = std::abs(inDamage);
    surpressed = inSurpressed;
    SetDefaultMessageIfNeeded();
    SetDeltaTime(deltaTimeIn);
}

void FCustomHitResult::SetupHitResult(
    FVector &customHitPoint,
    int inDamage,
    bool inSurpressed,
    float deltaTimeIn
){
    SetupHitResult(inDamage, inSurpressed, deltaTimeIn);
    hitPoint = customHitPoint;
    hitPointSetup = true;
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
    result = inResult;
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

void FCustomHitResult::SetDeltaTime(float deltaTimeIn){
    savedDeltaTime = deltaTimeIn;
}

float FCustomHitResult::DeltaTime() const {
    return savedDeltaTime;
}

int FCustomHitResult::Damage() const {
    return damage;
}
bool FCustomHitResult::IsSurpressed() const {
    return surpressed;
}

FVector &FCustomHitResult::HitPoint(){
    return hitPoint;
}

const FVector &FCustomHitResult::Direction() const {
    return direction;
}

AActor *FCustomHitResult::GetActor() const {
    return result.GetActor();
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