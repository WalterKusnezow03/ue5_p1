// Fill out your copyright notice in the Description page of Project Settings.



#include "Outpost.h"
 #include "GameCore/PlayerInfo/PlayerInfo.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/_world/worldLevel.h"
#include "GameCore/util/FVectorUtil.h"
#include "OutpostAlarmPole.h"
#include "DebugPlugin/DebugHelper.h"
#include <cstdlib>
#include "p2/ui/_uiSimulation/UiSimulation.h"
#include "p2/ui/_uiSimulation/EUiEvent.h"



// Sets default values
AOutpost::AOutpost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//so dass es sich bewegen kann
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("outpostRoot"));
	RootComponent = SceneRoot;

	isLiberated = false;
}

// Called when the game starts or when spawned
/// CAUTION: Will create 5 human entities on begin play, on spawn!
void AOutpost::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOutpost::init(){
	createEntity(5, teamEnum::enemyTeam);
}

// --- on end play / reload world ---
void AOutpost::ClearAllOnEndPlay(){
	releaseAll();
	teamLeaders.clear();
	ClearAlarmPoles();
}

void AOutpost::ClearAlarmPoles(){

	for (int i = 0; i < alarmPoles.size(); i++){
		if(AOutpostAlarmPole *current = alarmPoles[i]){
			current->ResetParent();
			current->Destroy(); //to be tested ALL
			alarmPoles[i] = nullptr;
		}
	}

	alarmPoles.clear();
}



// Called every frame
void AOutpost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(isLiberated){
		return;
	}

	FVector playerlocation = PlayerInfo::playerLocation();
	if(isInRange(playerlocation)){
		//spawn if needed
		initEntitiesIfNeeded();
		switchPlayerEnteredStatus(true);
	}else{
		//despawn if needed
		releaseAll();
		switchPlayerEnteredStatus(false);
	}


	//show 50 meter outline
	if(false){
		FVector outlineAdd(0, MAXDISTANCE_RADIUS, 0);
		FVector outlineUp(0, 0, MAXDISTANCE_RADIUS);
		FVector corner = GetActorLocation();
		FVector draw = corner + outlineAdd;
		FVector drawEnd = draw + outlineUp;
		DebugHelper::showLineBetween(GetWorld(), corner, draw, FColor::Red);
		DebugHelper::showLineBetween(GetWorld(), draw, drawEnd, FColor::Red);
	}
	
}


/// @brief will react to a player entered status
/// @param status 
void AOutpost::switchPlayerEnteredStatus(bool status){

	if(status != playerEntered){
		playerEntered = status;
		if(playerEntered){
			moveAllEntitiesToGroundOnPlayerEnterArea();
		}

		UpdatePlayerEnteredUi();

				//Debug
		/*
		if(playerEntered){
			DebugHelper::showScreenMessage("player entered the area");
		}else{
			DebugHelper::showScreenMessage("player left the area");
		}*/
	}

}

void AOutpost::UpdatePlayerEnteredUi(){
    FString message = playerEntered ? "Outpost Area Entered" : "Outpost Area left";
    EUiEvent type = playerEntered ? EUiEvent::HudTopText : EUiEvent::HudTopTextTimed;
    AworldLevel::uiSimulation.Notify(type, message);
}



void AOutpost::moveAllEntitiesToGroundOnPlayerEnterArea(){
	for(auto &pair : teamMap){
		std::vector<AHumanEntityScript *> &vec = pair.second;
		for (int i = 0; i < vec.size(); i++)
		{
			AHumanEntityScript *h = vec.at(i);
			if(h != nullptr){
				h->projectActorToGround();
			}
		}
	}
}








/// @brief returns if a vector is in the max range of the outpost
/// @param vec to check
/// @return within range
bool AOutpost::isInRange(FVector &vec){
	FVector ownLocation = GetActorLocation();

	return (FVector::Dist(vec, ownLocation) <= MAXDISTANCE_RADIUS);
}


/// @brief searches an entity in own list and removes it, releases entity to manager
/// @param entity 
void AOutpost::releaseEntity(AHumanEntityScript *entity){
	if(entity != nullptr){

		//new team based
		removeFromMap(entity);

		if(EntityManager *e = AworldLevel::entityManager()){
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

		//pick a new team leader if needed yet
		pickTeamLeaderIfNeeded(team);
	}
}


/// @brief creates an human entity and subscribes it
/// @param team team of the entity to set
void AOutpost::createEntity(teamEnum team){
	if(EntityManager *e = AworldLevel::entityManager()){
	
		FVector pos = randomOffsetFromActorLocation(400);
		pos.Z += 100;

		AHumanEntityScript *human = e->spawnHumanEntity(GetWorld(), pos, team);
		if(human != nullptr){
			//human->setTeam(team);
			subscribe(human);

			FVector posCopy = human->GetActorLocation();

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

FVector AOutpost::randomOffsetFromActorLocation(int range){
	return GetActorLocation() + randomOffset(range);
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

		DebugHelper::showScreenMessage("outpost init!", FColor::Orange);
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

			if(human){
				human->despawn();
			}
			
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
		replaceTeamLeaderIfNeededOnRemoveOf(human);
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





/**
 * 
 * --- team leader section ---
 * 
 */

///@brief offsets the target position if the bot is not the team leader
void AOutpost::validatePlayerTargetMovingPosition(
	AHumanEntityScript *botPointer, 
	FVector &position
){
	if(!isTeamLeader(botPointer)){
		//random offset
		int range = 500; //3m
		FVector randomOffset = FVectorUtil::randomOffset(range);
		position += randomOffset;
		DebugHelper::logMessage("debugOutpost - was NOT team leader!");
	}else{
		DebugHelper::logMessage("debugOutpost - was team leader!");
	}
}

///@brief will tell if a bot is the current team leader of a team
bool AOutpost::isTeamLeader(AHumanEntityScript *bot){
	if(bot != nullptr){
		teamEnum team = bot->getTeam();
		if(teamLeaders.find(team) != teamLeaders.end()){
			return teamLeaders[team] == bot;
		}
	}
	return false;
}

///@brief replace the team leader before removing this bot!
void AOutpost::replaceTeamLeaderIfNeededOnRemoveOf(AHumanEntityScript *botPointer){
	if(botPointer != nullptr){
		teamEnum botTeam = botPointer->getTeam();
		if (isTeamLeader(botPointer))
		{
			teamLeaders[botTeam] = nullptr;
		}
		pickTeamLeaderIfNeeded(botTeam);
	}
}

///@brief tries to pick a new team leader if the current is nullptr or not found
void AOutpost::pickTeamLeaderIfNeeded(teamEnum team){
	if(newTeamLeaderNeeded(team)){

		//pick new random from team
		if(teamMap.find(team) != teamMap.end()){
			std::vector<AHumanEntityScript *> &vec = teamMap[team];
			if(vec.size() > 0){
				int index = FVectorUtil::randomNumber(0, vec.size()) % vec.size();
				AHumanEntityScript *pointer = vec[index];
				if(pointer != nullptr){
					teamLeaders[team] = pointer;
				}
			}
		}
	}
}

///@brief tells if there is no team leader or the pointer is nullptr (not valid anymore)
bool AOutpost::newTeamLeaderNeeded(teamEnum team){
	if(teamLeaders.find(team) == teamLeaders.end()){
		return true;
	}
	if(teamLeaders[team] == nullptr){
		return true;
	}
	return false;
}






/**
 * alarm poles section
 */
void AOutpost::createAlarmPolesIfNeeded(){
	if(alarmPoles.size() == 0){
		int num = 1;
		for (int i = 0; i < num; i++){
			FVector location = randomOffsetFromActorLocation(500);

			AOutpostAlarmPole *pole = AOutpostAlarmPole::Construct(GetWorld(), location);
			if (pole)
			{
				pole->enableAlarmFunction();
			}
		}
	}
}


void AOutpost::resetAlarmPoles(){
	if(alarmPoles.size() > 0){
		for (int i = 0; i < alarmPoles.size(); i++){
			AOutpostAlarmPole *pole = alarmPoles[i];
			if(pole){
				pole->enableAlarmFunction();
			}
		}
	}
}

