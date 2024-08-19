// Fill out your copyright notice in the Description page of Project Settings.



#include "Outpost.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/entities/HumanEntityScript.h"
#include <cstdlib>



// Sets default values
AOutpost::AOutpost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isLiberated = false;
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

	if(isLiberated){
		return;
	}

	if(referenceManager *r = referenceManager::instance()){
		AplayerScript *player = r->getPlayerPointer();
		if(player != nullptr){
			FVector location = player->GetActorLocation();

			//float dist = (FVector::Dist(location, GetActorLocation()) / 100);
			//DebugHelper::showScreenMessage(FString::Printf(TEXT("dist outpost %f"), dist));

			bool inRange = isInRange(location);
			if(!inRange){
				//despawn
				DebugHelper::showScreenMessage("player left the area");
				releaseAll();
			}
			else
			{
				//spawn if needed
				initEntitiesIfNeeded();
			}
		}
	}
	
}

/// @brief returns if a vector is in the max range of the outpost
/// @param vec to check
/// @return within range
bool AOutpost::isInRange(FVector &vec){
	FVector ownLocation = GetActorLocation();

	//return ((FVector::Dist(vec, ownLocation) / 100) <= MAXDISTANCE_METERS);

	return (FVector::Dist(vec, ownLocation) <= MAXDISTANCE);
}


/// @brief searches an entity in own list and removes it, releases entity to manager
/// @param entity 
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


/// @brief creates an human entity and subscribes it
void AOutpost::createEntity(){
	if(EntityManager *e = EntityManager::instance()){
		FVector pos = GetActorLocation();
		pos.Z += 100;
		pos += randomOffset(200);
		AHumanEntityScript *human = e->spawnHumanEntity(GetWorld(), pos);
		if(human != nullptr){
			subscribe(human);
		}
	}
}

FVector AOutpost::randomOffset(int range){
	int x = std::rand();
	int y = std::rand();
	x = x % range;
	y = y % range;
	return FVector(x, y, 0);
}

void AOutpost::createEntity(int count){
	for (int i = 0; i < count; i++){
		createEntity();
	}
}

/// @brief will init entities if none are in the array 
/// AND is not liberated!
void AOutpost::initEntitiesIfNeeded(){
	if(myEntities.size() <= 0 && !isLiberated){
		createEntity(5);
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




/// @brief liberate the outpost if no entities are left (dead or despawned, no difference)
void AOutpost::liberate(){
	if(myEntities.size() <= 0){
		alarmEnabled = false;
		alertEnabled = false;
		isLiberated = true; 

		//testing recreation --> testing completed, works as expected with generic entity manager
		isLiberated = false;
		initEntitiesIfNeeded();

		DebugHelper::showScreenMessage("liberated outpost", FColor::Yellow);
	}
}



/// @brief will despawn all entities of the outpost
void AOutpost::releaseAll(){
	if(myEntities.size() > 0){
		
		while(myEntities.size() > 0){
			AHumanEntityScript *human = myEntities.front();
			human->despawn();
			// releaseEntity(human); //will find and release the targeted entity
		}
	}
}

