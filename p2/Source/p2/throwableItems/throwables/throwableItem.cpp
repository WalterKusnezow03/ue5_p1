// Fill out your copyright notice in the Description page of Project Settings.


#include "throwableItem.h"
#include "p2/weapon/throwerWeapon/enum/throwableEnum.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "GameCore/util/AActorUtil.h"
#include "Components/StaticMeshComponent.h"

void AthrowableItem::BeginPlay(){
	Super::BeginPlay();
	enablePhysics(false);
}

// Called every frame
void AthrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/// @brief throws the item into a direction, shows it enables physics
/// @param start 
/// @param direction is normalized internally.
void AthrowableItem::throwIntoDirection(FVector start, FVector direction){
	if (!isThrown){
		Super::throwIntoDirection(start, direction);
		AddImpulse(direction, velocityThrow);
	}
}

void AthrowableItem::AddImpulse(const FVector &direction, float velocity){
	if(mesh){ 
		mesh->AddImpulse(direction.GetSafeNormal() * velocity);
	}
}







/// @brief enable or disable physics for this actor (and all static mesh components)
/// @param enable bool
void AthrowableItem::enablePhysics(bool enable){
	if(mesh){
		mesh->SetSimulatePhysics(enable);
	}
	bPhysicsEnabled = true;
}

bool AthrowableItem::PhysicsEnabled(){
	return bPhysicsEnabled;
}

float AthrowableItem::Velocity(){
	FVector Velocity(0, 0, 0);
	if(mesh && bPhysicsEnabled){
		Velocity = mesh->GetPhysicsLinearVelocity();
	}
	return Velocity.Size();
}

/// @brief resets thrown status, physics and visibilty to true 
void AthrowableItem::reset(){
	Super::reset();
	enablePhysics(false);
}



/// @brief will release the item to the entity manager
void AthrowableItem::release(){
	enablePhysics(false);
	Super::release();
}