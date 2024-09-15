// Fill out your copyright notice in the Description page of Project Settings.



#include "Outpost.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/player/teamEnum.h"
#include "p2/_world/worldLevel.h"
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
	createEntity(5, teamEnum::enemyTeam);
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


	//show 50 meter outline
	FVector outlineAdd(0, MAXDISTANCE_RADIUS, 0);
	FVector outlineUp(0, 0, MAXDISTANCE_RADIUS);
	FVector corner = GetActorLocation();
	FVector draw = corner + outlineAdd;
	FVector drawEnd = draw + outlineUp;
	DebugHelper::showLineBetween(GetWorld(), corner, draw, FColor::Red);
	DebugHelper::showLineBetween(GetWorld(), draw, drawEnd, FColor::Red);
}

/// @brief returns if a vector is in the max range of the outpost
/// @param vec to check
/// @return within range
bool AOutpost::isInRange(FVector &vec){
	FVector ownLocation = GetActorLocation();

	//return ((FVector::Dist(vec, ownLocation) / 100) <= MAXDISTANCE_METERS);

	return (FVector::Dist(vec, ownLocation) <= MAXDISTANCE_RADIUS);
}


/// @brief searches an entity in own list and removes it, releases entity to manager
/// @param entity 
void AOutpost::releaseEntity(AHumanEntityScript *entity){
	if(entity != nullptr){

		//new team based
		removeFromMap(entity);
		//teamEnum team = entity->getTeam();
		//removeFromVec(entity, getVectorFor(team));


		if(EntityManager *e = worldLevel::entityManager()){
			e->add(entity);
		}
		
		//liberate if all dead --> must be refactured to enemy team
		tryliberate();
		
	}
}

/// @brief subscribes an human to the outpost
/// @param entity human in, not nullptr
/// @return outpost
void AOutpost::subscribe(AHumanEntityScript *entity){
	if(entity != nullptr){

		teamEnum team = entity->getTeam();
		std::vector<AHumanEntityScript *> &vector = getVectorReferenceFor(team);
		vector.push_back(entity);

		//myEntities.push_back(entity); //weil instanz variable mit .punkt
		entity->setOutpost(this);

		DebugHelper::showScreenMessage("subscribed an entity!", FColor::Red);
	}
}


/// @brief creates an human entity and subscribes it
/// @param team team of the entity to set
void AOutpost::createEntity(teamEnum team){
	if(EntityManager *e = worldLevel::entityManager()){
		FVector pos = GetActorLocation();
		pos.Z += 100;
		pos += randomOffset(400);
		AHumanEntityScript *human = e->spawnHumanEntity(GetWorld(), pos, team);
		if(human != nullptr){
			//human->setTeam(team);
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

/// @brief creates entities with a count
/// @param count number of enteties
/// @param team team of the enties
void AOutpost::createEntity(int count, teamEnum team){
	for (int i = 0; i < count; i++){
		createEntity(team);
	}
}

/// @brief will init entities if none are in the array 
/// AND is not liberated!
void AOutpost::initEntitiesIfNeeded(){
	
	if(getVectorReferenceFor(teamEnum::enemyTeam).size() <= 0){
		createEntity(5, teamEnum::enemyTeam);
	}
	
}



/// @brief lowers the spotting time for all entites
void AOutpost::alertAll(){

	if (!alertEnabled)
	{
		alertEnabled = true;

		//alert all enteties, independant of team
		for(auto &pair : teamMap){

			std::vector<AHumanEntityScript *> &vec = pair.second;
			for (int i = 0; i < vec.size(); i++)
			{
				AHumanEntityScript *h = vec.at(i);
				if(h != nullptr){
					h->alert();
				}
			}
		}


		
	}
}


/// @brief sets the spotting status to true for all entites
void AOutpost::alarmAll(){

	if(!alarmEnabled){
		alarmEnabled = true;

		//enable alarm for all
		for(auto &pair : teamMap){
			std::vector<AHumanEntityScript *> &vec = pair.second;
			for (int i = 0; i < vec.size(); i++)
			{
				AHumanEntityScript *h = vec.at(i);
				if(h != nullptr){
					h->alarm();
				}
			}
		}
		
	}
}




/// @brief liberate the outpost if no entities are left (dead or despawned, no difference)
void AOutpost::tryliberate(){
	if(getVectorReferenceFor(teamEnum::enemyTeam).size() <= 0){ //a oder w = w
		alarmEnabled = false;
		alertEnabled = false;
		isLiberated = true; 

		
		DebugHelper::showScreenMessage("liberated outpost", FColor::Yellow);
	}
}



/// @brief will despawn all entities of the outpost
void AOutpost::releaseAll(){
	
	for(auto &pair : teamMap){

		std::vector<AHumanEntityScript *> &vec = pair.second;
		while(vec.size() > 0){
			AHumanEntityScript *human = vec.back();
			vec.pop_back();
			human->despawn();
			// releaseEntity(human); //will find and release the targeted entity
		}
	}
	//hot fix here, testing needed
	alarmEnabled = false;
	alertEnabled = false;
	isLiberated = false; //DO NOT LIBERATE WHEN DESPAWN ALL
}

/**
 * 
 * TEAM MAP / VECTOR METHODS
 * 
 * 
 */

/// @brief removes a human from the map from the correct vector
/// @param human 
void AOutpost::removeFromMap(AHumanEntityScript *human){
	if(human != nullptr){
		teamEnum team = human->getTeam();
		removeFromVec(human, getVectorReferenceFor(team));
	}
}

void AOutpost::addToMap(AHumanEntityScript *human){
	if(human != nullptr){
		teamEnum team = human->getTeam();
		//removeFromVec(human, getVectorFor(team));
		std::vector<AHumanEntityScript *> &vecReference = getVectorReferenceFor(team);
		vecReference.push_back(human);
	}
}


/// @brief theres no need to worry about thread safety because this method will be only called from
/// game thread and no issues should arise
/// @param human human to remove
/// @param vec vector to remove from
void AOutpost::removeFromVec(AHumanEntityScript *human, std::vector<AHumanEntityScript*> &vec){
	if(human != nullptr){
		for (int i = 0; i < vec.size(); i++){
			if(vec.at(i) == human){
				vec.erase(vec.begin() + i);
				return;
			}
		}
	}
}

/// @brief returns the correct vector for a team for this outpost
/// @param team team to get
/// @return vector of the alive entites for the team requested
std::vector<AHumanEntityScript *>& AOutpost::getVectorReferenceFor(teamEnum team){
	if(teamMap.find(team) == teamMap.end()){
		std::vector<AHumanEntityScript *> vec; 
		teamMap[team] = vec;
	}

	std::vector<AHumanEntityScript *> &ref = teamMap[team];
	return ref;
}
