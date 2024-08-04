// Fill out your copyright notice in the Description page of Project Settings.

#include "playerScript.h"
#include "EntityScript.h"

// Sets default values
AEntityScript::AEntityScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = 100;

	playerPointer = nullptr;

	defaultSpottingTime = 5;
}

// Called when the game starts or when spawned
void AEntityScript::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEntityScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool canSeePlayer = performRaycast(playerPointer);

	
	if(canSeePlayer){
		
		if(spottedPlayer){
			//attack if can see and spotted
			
		}else{
			//update time if can see, but not spotted
			updateSpottingTime(DeltaTime);
		}


	}else{
		if(spottedPlayer){
			//follow
		}else{
			//cant see, hasnt spotted: reset
			setSpottingTime(defaultSpottingTime);
		}
	}
	
}


//allows the entity to take damage
void AEntityScript::takedamage(int d){
	health -= d;
	if(health <= 0){
		d = 0;
		die();
	}
}


void AEntityScript::die(){

}



bool AEntityScript::performRaycast(AActor *target){
	if(target){
		return performRaycast(*target); //dereference for the ref
	}
	return false;
}

bool AEntityScript::performRaycast(AActor &target) //because a reference is expected it must be valid
{


	// Get the camera location and rotation
    FVector End = target.GetActorLocation();

    // Define the start and end vectors for the raycast
    FVector Start = this->GetActorLocation();

	float distance = FVector::Dist(Start, End);

	// Perform the raycast
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore the character itself

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);



    // If the raycast hit something, log the hit actor's name
    if (bHit)
    {
		AActor* actor = HitResult.GetActor();
		if(actor != nullptr){
			
			//die POINTER vergleichen, einmal den pointer, einmal die &adresse
			if(actor == &target){
				return false;
			}
		}

		
    }
	return false;
}

/**
 * sets the spotting time a given value
 */
void AEntityScript::setSpottingTime(float time){
	spottingTimeLeft = time;
}

/**
 * update the spotting time
 */
void AEntityScript::updateSpottingTime(float deltaTime){
	if(spottingTimeLeft > 0){
		spottingTimeLeft -= deltaTime;
	}else{
		spottingTimeLeft = 0;
		spottedPlayer = true;
	}
}