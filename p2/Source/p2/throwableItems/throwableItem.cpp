// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/throwableItems/throwableItem.h"
#include "throwableEnum.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AthrowableItem::AthrowableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isThrown = false;

	//throwableType = throwableEnum::rock_enum;
}

// Called when the game starts or when spawned
void AthrowableItem::BeginPlay()
{
	Super::BeginPlay();
	enablePhysics(false);
}

// Called every frame
void AthrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AthrowableItem::setLocation(FVector location){
	if(!isThrown){
		SetActorLocation(location);
	}
	
}
void AthrowableItem::setRotation(FRotator rotation){
	if(!isThrown){
		SetActorRotation(rotation);
	}
}


/// @brief throws the item into a direction
/// @param start 
/// @param direction 
void AthrowableItem::throwIntoDirection(FVector start, FVector direction){
	
	
	if (!isThrown)
	{
		DebugHelper::showScreenMessage("throw!");
		isThrown = true;
		int velocity = 1000; //verlängern
		

		SetActorLocation(start);
		enablePhysics(true);

		UStaticMeshComponent *mesh = FindComponentByClass<UStaticMeshComponent>();
		if(mesh ){ //&& mesh->IsSimulatingPhysics()
			mesh->AddImpulse(direction * velocity);
		}
	}
}

/// @brief get the type of this throwable item
/// @return type
throwableEnum AthrowableItem::getType(){
	return throwableType;
}



void AthrowableItem::enablePhysics(bool enable){
	TArray<UStaticMeshComponent *> components;
	GetComponents<UStaticMeshComponent>(components);
	for (int i = 0; i < components.Num(); i++){
		UStaticMeshComponent *c = components[i];
		if(c != nullptr){
			c->SetSimulatePhysics(enable);
		}
	}
}