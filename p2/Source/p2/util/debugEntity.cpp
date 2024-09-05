// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/entities/HumanEntityScript.h"
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
		FVector location1 = GetActorLocation();
		location1.Z = 20;

		int metersLayout = 20;
		e->createALayout(GetWorld(), location1, metersLayout, metersLayout);

		return;

		int meters = 50;
        e->createTerrain(GetWorld(), meters);

		for (int i = 0; i < 1; i++){
			FVector location = GetActorLocation();
			location += FVectorUtil::randomOffset(700);
			AHumanEntityScript *a = e->spawnHumanEntity(GetWorld(), location);
			if(a != nullptr){
				a->init();
			}
		}
	}
}