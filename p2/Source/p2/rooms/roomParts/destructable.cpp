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

/// @brief damage method which will hide and disable the actor and create a splitted version
/// will not react to any damage if "destructable" bool is not enabled
/// @param d can be ignored
void Adestructable::takedamage(int d){
	if(!destructable){
		return;
	}

	FVector bottomCenter = GetActorLocation();
	int cmPerTile = 50;
	//material will be set somewhere else, no worries
	AcustomMeshActor::splitAndreplace(this, bottomCenter, cmPerTile, materialType); 


	//just disable actor for now, no remove	
	AActorUtil::showActor(*this, false);
	AActorUtil::enableColliderOnActor(*this, false);
}



void Adestructable::takedamage(int d, FVector &from){
	if(destructable){
		takedamage(d);
	}
}

