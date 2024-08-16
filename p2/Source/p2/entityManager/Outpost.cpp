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

/// @brief returns if a vector is in the max range of the outpost
/// @param vec to check
/// @return within range
bool AOutpost::isInRange(FVector &vec){
	FVector ownLocation = GetActorLocation();
	return (FVector::Dist(vec, ownLocation) <= MAXDISTANCE);
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
		
		//liberate if all dead
		if(myEntities.size() <= 0){
			liberate();
		}
	}
}

/// @brief subscribes an human to the outpost
/// @param entity human in, not nullptr
/// @return outpost
void AOutpost::subscribe(AHumanEntityScript *entity){
	if(entity != nullptr){
		myEntities.push_back(entity); //weil instanz variable mit .punkt
		entity->setOutpost(this);

		DebugHelper::showScreenMessage("subscribed an entity!", FColor::Red);
	}
}



void AOutpost::createEntity(){
	if(EntityManager *e = EntityManager::instance()){
		FVector pos = GetActorLocation();
		AHumanEntityScript *human = e->spawnHumanEntity(GetWorld(), pos);
		if(human != nullptr){
			subscribe(human);
			//myEntities.push_back(human);
			//human->setOutpost(this);
		}
	}
}


/// @brief lowers the spotting time for all entites
void AOutpost::alertAll(){

	if (!alertEnabled)
	{
		alertEnabled = true;
		for (int i = 0; i < myEntities.size(); i++)
		{
			AHumanEntityScript *h = myEntities.at(i);
			if(h != nullptr){
				h->alert();
			}
		}
	}
}


/// @brief sets the spotting status to true for all entites
void AOutpost::alarmAll(){

	if(!alarmEnabled){
		alarmEnabled = true;
		for (int i = 0; i < myEntities.size(); i++)
		{
			AHumanEntityScript *h = myEntities.at(i);
			if(h != nullptr){
				h->alarm();
			}
		}
	}
}




/// @brief liberate the outpost if no entities are left
void AOutpost::liberate(){
	if(myEntities.size() <= 0){
		alarmEnabled = false;
		alertEnabled = false;
	}
}