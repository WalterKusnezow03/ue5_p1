#include "VFXActor.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"


AVFXActor::AVFXActor() : Super(){
    PrimaryActorTick.bCanEverTick = true;
}

void AVFXActor::BeginPlay(){
    Super::BeginPlay();
    FindAllComponentsIfEmpty();
}

void AVFXActor::Tick(float deltatime){
    Super::Tick(deltatime);
    TickAllComponents(deltatime);
}

void AVFXActor::TickAllComponents(float DeltaTime){
    FindAllComponentsIfEmpty();
    for (int i = 0; i < components.Num(); i++){
        if(UNiagaraComponentCustom *current = components[i]){
            current->TickExternal(DeltaTime);
        }
    }
}





void AVFXActor::FindAllComponentsIfEmpty(){
    if(components.Num() == 0){
        TFindAllChildsByType<UNiagaraComponentCustom>(components);
    }
}

void AVFXActor::Init(){
    FindAllComponentsIfEmpty();
    PlayAllEffects();
}

void AVFXActor::PlayAllEffects(){
    ResetAllEffects();
    for (int i = 0; i < components.Num(); i++){
        if(UNiagaraComponentCustom *current = components[i]){
            current->StartEffect();
        }
    }
}

void AVFXActor::ResetAllEffects(){
    for (int i = 0; i < components.Num(); i++){
        if(UNiagaraComponentCustom *current = components[i]){
            current->Reset();
        }
    }
}

void AVFXActor::ReleaseToEntityManager(){
    ResetAllEffects();
    if(EntityManager *e = AworldLevel::entityManager()){
        e->add(this);
    }
}