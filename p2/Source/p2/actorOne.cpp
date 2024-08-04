// Fill out your copyright notice in the Description page of Project Settings.


#include "actorOne.h"

// Sets default values
AactorOne::AactorOne()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AactorOne::BeginPlay()
{
	Super::BeginPlay();


	startPoint = this->GetActorLocation();
	endPoint = startPoint;
	endPoint.Z += 400;
	endPoint.Y += 1000;


}

// Called every frame
void AactorOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetActorLocation(endPoint);

	SetActorLocation(transformTo(DeltaTime, this->GetActorLocation(), endPoint));
}

FVector AactorOne::transformTo(float time, FVector currentLocation, FVector toLocation){

	float distance = FVector::Dist(currentLocation, toLocation);
	if(distance <= 1){
		return toLocation;
	}

	// Direction vector from current location to target location
    FVector dir = (toLocation - currentLocation).GetSafeNormal(); // Normalize the direction vector

    // Move from current location towards target location by stepDistance
    FVector newLocation = currentLocation + (dir / time);

	return newLocation;

}

