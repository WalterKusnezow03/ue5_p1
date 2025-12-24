// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetPlugin/gameStart/assetEnums/throwableEnum.h"
#include "p2/throwableItems/grenadeItem.h"
#include "p2/entityManager/AlertManager.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
#include "DebugPlugin/DebugHelper.h"
#include "CoreMath/animation/timer/timer.h"

AgrenadeItem::AgrenadeItem(){
    throwableType = throwableEnum::greneade_enum;
    isDetonated = false;
}

void AgrenadeItem::BeginPlay(){
    Super::BeginPlay();
    explosiveHelper.Setup(EXPLOSION_RADIUS, DAMAGE, DAMAGE_RADIUS);
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
    explosiveHelper.detonate(location, GetWorld());
    isDetonated = true;
    //hide after detonate
    show(false);
    Super::release(); //release to entity manager
}
