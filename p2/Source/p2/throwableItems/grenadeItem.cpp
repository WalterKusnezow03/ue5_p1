// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/throwableItems/throwableEnum.h"
#include "p2/throwableItems/grenadeItem.h"
#include "p2/entityManager/AlertManager.h"
#include "p2/util/timer.h"

AgrenadeItem::AgrenadeItem(){
    throwableType = throwableEnum::greneade_enum;
    isDetonated = false;
}

void AgrenadeItem::BeginPlay(){
    Super::BeginPlay();
}

void AgrenadeItem::Tick(float deltaTime){
    Super::Tick(deltaTime);

    if(isThrown && !isDetonated){
        timer.Tick(deltaTime);
        if(timer.timesUp()){
            // explode;
            detonate();
        }
    }


}

/// @brief throws the grenade into a direction
/// @param start 
/// @param direction 
void AgrenadeItem::throwIntoDirection(FVector start, FVector direction){
    if(!isThrown){
        isDetonated = false;
        Super::throwIntoDirection(start, direction);

        //begin timer (IV)
        timer.Begin(1.2f);
    }
}

/// @brief detonates the grenade
void AgrenadeItem::detonate(){

    FVector location = GetActorLocation();

    AlertManager::damageAndAlertInArea(GetWorld(), GetActorLocation(), EXPLOSION_RADIUS, DAMAGE, EXPLOSION_RADIUS / 10);
    isDetonated = true;
    DebugHelper::showScreenMessage("grenade detonate"); //works as expected

    float radius = 1000.0f; // 5 * 100 = 5m
    if(EntityManager *e = EntityManager::instance()){
        e->createExplosion(GetWorld(), location);
    }

    //hide after detonate
    show(false);
}
