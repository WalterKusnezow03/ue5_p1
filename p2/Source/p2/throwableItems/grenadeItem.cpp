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

void AgrenadeItem::throwIntoDirection(FVector start, FVector direction){
    if(!isThrown){
        isDetonated = false;
        Super::throwIntoDirection(start, direction);

        //begin timer (IV)
        timer.Begin(1.2f);
    }
}


void AgrenadeItem::detonate(){
    AlertManager::damageAndAlertInArea(GetWorld(), GetActorLocation(), EXPLOSION_RADIUS, DAMAGE);
    isDetonated = true;
    DebugHelper::showScreenMessage("grenade detonate"); //works as expected
}
