// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/entities/customIk/BoneIk.h"
#include "p2/entities/customIk/abstractKinematicFunctions.h"
#include "p2/entities/customIk/IkActor.h"
#include "p2/util/debugEntity.h"

// Sets default values
AdebugEntity::AdebugEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AdebugEntity::BeginPlay()
{
	Super::BeginPlay();
	clicked = false;
	takedamage(0);
	spawnCustomIkActor();
}

// Called every frame
void AdebugEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	debugFunction(DeltaTime);
}

void AdebugEntity::takedamage(int d){
	if(!clicked){
		clicked = true;
		//debugFunction();
	}
}
void AdebugEntity::takedamage(int d, FVector &from){
	takedamage(d);
}

/// @brief sets the team, is empyt here, will not be set
/// @param team 
void AdebugEntity::setTeam(teamEnum team){
}

/// @brief will always return neutralTeam
/// @return 
teamEnum AdebugEntity::getTeam(){
	return teamEnum::neutralTeam;
}




void AdebugEntity::debugFunction(){
	
}


void AdebugEntity::debugFunction(float deltaTime){

}




/**
 * 
 * 
 * TESTING NEEDED FOR CUSTOM IK ACTOR SPAWNING
 * 
 * 
 */
void AdebugEntity::spawnCustomIkActor(){
	UWorld *world = GetWorld();
	if(world){
		FVector offset = FVector(300, 0, 200) + GetActorLocation();
		FRotator r;
		AIkActor *SpawnedActor = world->SpawnActor<AIkActor>(AIkActor::StaticClass(), offset, r);
		if(SpawnedActor != nullptr){
			SpawnedActor->SetActorLocation(offset);
		}
	}
	
}