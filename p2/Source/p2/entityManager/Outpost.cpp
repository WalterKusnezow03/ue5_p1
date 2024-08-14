// Fill out your copyright notice in the Description page of Project Settings.



#include "Outpost.h"
#include "p2/entities/HumanEntityScript.h"



// Sets default values
AOutpost::AOutpost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOutpost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOutpost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AOutpost::releaseEntity(AHumanEntityScript *entity){
	if(entity != nullptr){
		for (int i = 0; i < myEntities.size(); i++){
			if(myEntities.at(i) == entity){
				myEntities.erase(myEntities.begin() + i);
				break;
			}
		}

		if(EntityManager *e = EntityManager::instance()){
			e->add(entity);
		}
	}
}

AOutpost* AOutpost::subscribe(AHumanEntityScript *entity){
	if(entity != nullptr){
		myEntities.push_back(entity); //weil instanz variable mit .punkt
		return this;
	}
	return nullptr;
}



void AOutpost::createEntity(){
	if(EntityManager *e = EntityManager::instance()){
		FVector pos = GetActorLocation();
		AHumanEntityScript *human = e->spawnHumanEntity(GetWorld(), pos);
		if(human != nullptr){
			myEntities.push_back(human);
			human->setOutpost(this);
		}
	}
}


/// @brief sets the spotting status to true for all entites
void AOutpost::alertAll(){

	float newTime = 2;

	if (!alertEnabled)
	{
		for (int i = 0; i < myEntities.size(); i++){
			AHumanEntityScript *h = myEntities.at(i);
			if(h != nullptr){
				h->alert();
			}
		}
	}
}


/// @brief resets the spotting time for all entites
void AOutpost::alarmAll(){

	if(!alarmEnabled){
		for (int i = 0; i < myEntities.size(); i++){
			AHumanEntityScript *h = myEntities.at(i);
			if(h != nullptr){
				h->alarm();
			}
		}
	}
}