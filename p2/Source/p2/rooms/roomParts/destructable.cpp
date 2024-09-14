// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/util/AActorUtil.h"
#include "destructable.h"

// Sets default values
Adestructable::Adestructable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Adestructable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Adestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}





void Adestructable::setTeam(teamEnum val){

}
teamEnum Adestructable::getTeam(){
	return teamEnum::neutralTeam;//all can damage
}

void Adestructable::takedamage(int d){
	
	FVector bottomCenter = GetActorLocation();
	int cmPerTile = 25;
	//material will be set somewhere else, no worries
	AcustomMeshActor::splitAndreplace(this, bottomCenter, 25, materialType); 


	//just disable actor for now, no remove	
	AActorUtil::showActor(*this, false);
	AActorUtil::enableColliderOnActor(*this, false);
}


void Adestructable::takedamage(int d, FVector &from){
	takedamage(d);
}

