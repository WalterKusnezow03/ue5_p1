// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchableItemBase.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "GameCore/util/AActorUtil.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALaunchableItemBase::ALaunchableItemBase() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isThrown = false;

	//throwableType = throwableEnum::rock_enum;
}

// Called when the game starts or when spawned
void ALaunchableItemBase::BeginPlay()
{
	Super::BeginPlay();
	reset();
	FindMeshOnBeginPlay();
}

void ALaunchableItemBase::FindMeshOnBeginPlay(){
	mesh = FindComponentByClass<UStaticMeshComponent>();
}



// Called every frame
void ALaunchableItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



/// @brief get the type of this throwable item
/// @return type
EThrowableEnum ALaunchableItemBase::getType(){
	return throwableType;
}




/// @brief throws the item into a direction, shows it enables physics
/// @param start 
/// @param direction is normalized internally.
void ALaunchableItemBase::throwIntoDirection(FVector start, FVector direction){
	if (!isThrown){
		isThrown = true;
		SetActorLocation(start);
		show(true);
        latestThrowingDirection = direction;
    }
}


/// @brief resets thrown status, physics and visibilty to true 
void ALaunchableItemBase::reset(){
	isThrown = false;
	show(true);
}

/// @brief shows or hides the item
/// @param show bool as described
void ALaunchableItemBase::show(bool show){
	SetActorHiddenInGame(!show);
}

/// @brief will release the item to the entity manager
void ALaunchableItemBase::release(){
    show(false);
    if (EntityManager *e = AworldLevel::entityManager())
    {
        e->add(this);
    }
}