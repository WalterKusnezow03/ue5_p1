// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/referenceManager.h"
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
}

// Called every frame
void AdebugEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AdebugEntity::takedamage(int d){
	if(!clicked){
		clicked = true;
		debugFunction();
	}
}
void AdebugEntity::setTeam(int team){

}
int AdebugEntity::getTeam(){
	return referenceManager::TEAM_NEUTRAL;
}

void AdebugEntity::debugFunction(){
	if(EntityManager *e = EntityManager::instance()){
		//FVector location(0, 0, 0);
		// e->createALayout(GetWorld(), location, 20, 20);

		int meters = 50;
        e->createTerrain(GetWorld(), meters);
	}
}