#include "CustomHitResult.h"

void FCustomHitResult::SetupHitResult(int damageIn){
    SetupHitResult(damageIn, false);
}

void FCustomHitResult::SetupHitResult(
    int inDamage,
    bool inSurpressed
){
    damage = std::abs(inDamage);
    surpressed = inSurpressed;
    SetDefaultMessageIfNeeded();
}

void FCustomHitResult::SetupHitResult(
    FVector &customHitPoint,
    int inDamage,
    bool inSurpressed
){
    SetupHitResult(inDamage, inSurpressed);
    hitPoint = customHitPoint;
    hitPointSetup = true;
}

void FCustomHitResult::SetupHitResult(
    FVector &customHitPoint,
    int inDamage
){
    SetupHitResult(customHitPoint, inDamage, false);
}

void FCustomHitResult::SetupHitResult(
    FHitResult &inResult, 
    int inDamage, 
    bool inSurpressed
){
    result = inResult;
    SetupHitResult(inResult.ImpactPoint, inDamage, inSurpressed);
}

void FCustomHitResult::SetupHitResult(
    FHitResult &inResult,
    int inDamage,
    bool inSurpressed,
    FVector &raycastStart
){
    SetupHitResult(inResult, inDamage, inSurpressed);
    UpdateDirectionAndDistance(raycastStart);
}

void FCustomHitResult::UpdateDirectionAndDistance(FVector &startingLocation){
    direction = hitPoint - startingLocation; // AB = B - A
    distance = direction.Size();
    direction = direction.GetSafeNormal();
}


int FCustomHitResult::Damage(){
    return damage;
}
bool FCustomHitResult::IsSurpressed(){
    return surpressed;
}

FVector &FCustomHitResult::HitPoint(){
    return hitPoint;
}

FVector &FCustomHitResult::Direction(){
    return direction;
}

float FCustomHitResult::Distance(){
    return distance;
}

bool FCustomHitResult::HasHitPoint(){
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