// Fill out your copyright notice in the Description page of Project Settings.


#include "sightScript.h"

// Sets default values
AsightScript::AsightScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//UE_LOG(LogTemp, Warning, TEXT("AsightScript Constructor: %s"), *GetName());
}

// Called when the game starts or when spawned
void AsightScript::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AsightScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// @brief returns the sight center.
/// @return position of the sight
FVector AsightScript::getSightCenter(){
	return GetActorLocation();
}
