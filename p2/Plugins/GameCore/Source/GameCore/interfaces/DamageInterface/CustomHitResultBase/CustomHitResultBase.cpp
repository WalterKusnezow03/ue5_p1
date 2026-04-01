#include "CustomHitResultBase.h"

void FCustomHitResultBase::SetupHitResult(int damageIn){
    SetupHitResult(damageIn, false);
}

void FCustomHitResultBase::SetupHitResult(
    int inDamage,
    bool inSurpressed,
    float deltaTimeIn
){
    damage = std::abs(inDamage);
    surpressed = inSurpressed;
    SetDeltaTime(deltaTimeIn);
}

void FCustomHitResultBase::SetupHitResult(
    int inDamage,
    bool inSurpressed,
    float deltaTimeIn,
    AActor *actor
){
    SetupHitResult(inDamage, inSurpressed, deltaTimeIn);
    actorResult = actor;
}

void FCustomHitResultBase::SetupHitResult(
    int inDamage,
    bool inSurpressed,
    float deltaTimeIn,
    AActor *actor,
    const FVector &directionIn
){
    SetupHitResult(inDamage, inSurpressed, deltaTimeIn, actor);
    direction = directionIn;
}

void FCustomHitResultBase::SetDeltaTime(float deltaTimeIn){
    savedDeltaTime = deltaTimeIn;
}

float FCustomHitResultBase::DeltaTime() const {
    return savedDeltaTime;
}

int FCustomHitResultBase::Damage() const {
    return damage;
}
bool FCustomHitResultBase::IsSurpressed() const {
    return surpressed;
}

AActor *FCustomHitResultBase::GetActor() const {
    return actorResult;
}
