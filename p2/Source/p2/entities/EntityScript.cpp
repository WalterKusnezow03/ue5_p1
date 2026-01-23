// Fill out your copyright notice in the Description page of Project Settings.
#include "EntityScript.h"

#include "p2/player/playerScript.h"
#include "PathFinder/pathFinding/PathFinder.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
#include "Kismet/KismetMathLibrary.h"
#include "DebugPlugin/DebugHelper.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "p2/entityManager/AlertManager.h"
#include "GameCore/team/teamEnum.h"

#include "GameCore/util/FVectorUtil.h"


#include "p2/ui/PlayerUi.h"


// Sets default values
AEntityScript::AEntityScript()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AEntityScript::BeginPlay()
{
	Super::BeginPlay();

	BeginPlayHumanoidController();

	init();
	
}




/// @brief will enable the entity for tick
/// set the player pointer, 
/// set the spotting time and
/// the entity team
void AEntityScript::init(){
	//DebugHelper::showScreenMessage("entity init");
	enableActiveStatus(true);
	AlertManager::subscribeToAlert(this);

	health = 100;
	spottedPlayer = false;
	canSeePlayer = false;

	playerPointer = nullptr;

	defaultSpottingTime = 5.0f;
	setSpottingTime(defaultSpottingTime);
	setupRaycastIgnoreParams();

	//set team
	setTeam(teamEnum::neutralTeam);

	projectActorToGround();
}





// Called every frame
void AEntityScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(AworldLevel::gameStateManager.GameStateIsPaused()){
		return;
	}
	
	if(debugStaticEntity){
		return;
	}

	//tick bone controller
	humanoidPluginController.Tick(DeltaTime);
	SetActorLocation(humanoidPluginController.GetLocation()); //override location for markers and pathfinding, all.
	//SetActorRotation(humanoidPluginController.GetRotation());

	if(AworldLevel::DebugSkelletonRecordMode()){ //worldLeevlBase method
		humanoidPluginController.setStateWalking();
		debugRecordRotationTimer.Tick(DeltaTime);
		if(debugRecordRotationTimer.timesUp()){
			debugRecordRotationTimer.Begin(30.0f, true);

			float degree = FVectorUtil::randomFloatNumber(-40, 40);
			humanoidPluginController.setupRotationForNextStep(MMatrix::degToRadian(degree));
		}
		return;
	}

	//only update if activated
	if(!isActivatedForUpdate()){
		return;
	}
	drawPath(); //debug

	
	


	//get player pointer if needed
	if(playerPointer == nullptr){
		referenceManager *i = referenceManager::instance();
		if(i){
			playerPointer = i->getPlayerPointer();
		}
	}
	if(playerPointer == nullptr){
		return;
	}

	canSeePlayer = false; //reset if not reserved for frane update
	

	// if not spotted yet, check angle, if angle ok, check vision
	if(
		withinVisionAngle(playerPointer) && 
		isWithinMaxRange(playerPointer->GetActorLocation())
	){
		//DebugHelper::showScreenMessage("player vision check");
		canSeePlayer = performRaycast(playerPointer);
	}



	//act based on vision
	if(canSeePlayer){
		if(!spottedPlayer){
			updateSpottingTime(DeltaTime);
		}
		DebugHelper::showScreenMessage("can see player");
	}
	else
	{
		//cant see player
		if(!spottedPlayer){
			//cant see, hasnt spotted: reset
			setSpottingTime(defaultSpottingTime);
		}
	}

	//moves towards player is spotted and cant see player
	//moveTowardsPlayer(DeltaTime);
	updatePathDelay(DeltaTime);
	actUponCurrentAction(DeltaTime);

	
}

//allows the entity to take damage
void AEntityScript::takedamage(int d){
	takedamage(d, false);
}
/// @brief hitpoint processing not specified, will only damage as default
/// @param d 
/// @param hitpoint 
void AEntityScript::takedamage(int d, FVector &hitpoint){
	takedamage(d, hitpoint, false);
}

///@brief main reaction method
void AEntityScript::takedamage(int d, bool surpressed){
	//showScreenMessage("enemy entity damage");
	health -= d;
	if(health <= 0){
		d = 0;
		die();
	}
	updateToReducedSpottingTimeIfNotSpottedYet();
}

void AEntityScript::takedamage(int d, FVector &hitpoint, bool surpressed){
	takedamage(d, surpressed);
}




/// @brief checks if an actor is within 180 degree range to own forward vector
/// @param target 
bool AEntityScript::withinVisionAngle(AActor *target){
	if(target != nullptr){

		//wenn das skalarpdoukt zweier vektoren 0 ergibt sind sie orthogonal zu einander
		//wenn das skalarprodukt zweier vektoren 1 ergibt sind sie paralell zu einander

		//FVector forward = GetActorForwardVector().GetSafeNormal();
		FVector forward = humanoidPluginController.lookDirection();		 
		FVector currentLocation = humanoidPluginController.GetLocation();

		//ab = b - a
		FVector toTarget = (target->GetActorLocation() - currentLocation).GetSafeNormal();

		float skalarprodukt = FVector::DotProduct(forward, toTarget);


		//mindestens orthogonal oder näher an der 1
		if(skalarprodukt >= 0.0f){
			return true;
		}
	}
	return false;
}



/// @brief sets up the ignore params just once for raycast to avoid unesecarry code in raycast method
void AEntityScript::setupRaycastIgnoreParams(){
	ignoreParams.AddIgnoredActor(this); // Ignore the character itself
	// Iterate and ignore all child actors
	TArray<AActor*> ChildActors;
	this->GetAttachedActors(ChildActors);

	for (AActor* ChildActor : ChildActors){
		ignoreParams.AddIgnoredActor(ChildActor);
	}
}



/// @brief returns if the distance to a entity is within the max range
/// @param vec 
/// @return 
bool AEntityScript::isWithinMaxRange(FVector vec){
	FVector currentLocation = humanoidPluginController.GetLocation(); 
	return (FVector::Dist(currentLocation, vec) <= MAXDISTANCE);
}

bool AEntityScript::isWithinCloseRange(FVector &vec){
	FVector currentLocation = humanoidPluginController.GetLocation();
	return (FVector::Dist(currentLocation, vec) <= MAXDISTANCE_CLOSERANGE);
}




/// @brief performs a raycast to the target and checks if "can see it"
/// @param target aactor from the scene
/// @return can see or not as bool
bool AEntityScript::performRaycast(AActor *target) //because a reference is expected it must be valid
{
	
	if (target != nullptr)
	{
		
		// Get the camera location and rotation
		FVector End = target->GetActorLocation();


		// Define the start and end vectors for the raycast
		FVector Start = this->GetActorLocation();

		FVector dir = (End - Start).GetSafeNormal();
		Start += dir * 100; //50cm

		//End = Start + dir * 1.1f; //safety reasons
		

		// Perform the raycast
		FHitResult HitResult;


		if(EntityManager *e = AworldLevel::entityManager()){
			//ignoreParams = e->getIgnoredRaycastParams(); //example for getting all
			ignoreParams = e->getIgnoredRaycastParams(getTeam());
		}

		ignoreParams.bTraceComplex = false;  //HIER LOWER RAYCAST DETAIL-> FASTER
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult, 
			Start, 
			End, 
			ECC_Visibility, 
			ignoreParams
		);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

		// If the raycast hit something, log the hit actor's name
		if (bHit)
		{
			AActor *actor = HitResult.GetActor();	
			if(actor == target){
				return true;
			}
		}
	}
	return false;
	
}




/// @brief performs a raycast in a direction, if hit the output will be saved in output (pass by ref)
/// @param direction direction to have
/// @param output output to save hitpoint in
/// @param cmLength max length in cm
/// @return hit or not at max distance
bool AEntityScript::performRaycast(FVector &direction, FVector &output, int cmLength) 
{
	
	// Define the start and end vectors for the raycast
	//FVector Start = this->GetActorLocation();

	FVector Start = humanoidPluginController.GetLocation(); 
	direction = direction.GetSafeNormal();
	Start += direction * 100; //50cm

	// Get the camera location and rotation
	FVector End = Start + direction * cmLength;

	// Perform the raycast
	FHitResult HitResult;

	if (EntityManager *e = AworldLevel::entityManager())
	{
		// ignoreParams = e->getIgnoredRaycastParams(); //example for getting all
		ignoreParams = e->getIgnoredRaycastParams();
	}
	ignoreParams.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, ignoreParams);
	
	// If the raycast hit something, log the hit actor's name
	if (bHit)
	{
		output = HitResult.ImpactPoint;
	}

	return bHit;
}



void AEntityScript::projectActorToGround(){
	int meters = 100 * 50;
	FVector Start = GetActorLocation() + FVector(0, 0, meters);

	// Get the camera location and rotation
	FVector End = GetActorLocation() - FVector(0, 0, meters);

	// Perform the raycast
	FHitResult HitResult;


	
	if (EntityManager *e = AworldLevel::entityManager())
	{
		// ignoreParams = e->getIgnoredRaycastParams(); //example for getting all
		ignoreParams = e->getIgnoredRaycastParams();
	}
	ignoreParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, ignoreParams);
	
	// If the raycast hit something, log the hit actor's name
	if(bHit){
		humanoidPluginController.SetLocation(HitResult.ImpactPoint);
		SetActorLocation(humanoidPluginController.GetLocation());


		//debug
		humanoidPluginController.EnableDebugLogExtended();

	}
	//rebuild legs.
	humanoidPluginController.ResetAndRebuild();
}










/**
 * sets the spotting time a given value
 */
void AEntityScript::setSpottingTime(float time){
	//spottingTimeLeft = time;
	spottingTimer.Begin(time);
}

/**
 * update the spotting time
 */
void AEntityScript::updateSpottingTime(float deltaTime){
	if(spottedPlayer){
		return;
	}
	spottingTimer.Tick(deltaTime);
	if(spottingTimer.timesUp()){
		spottedPlayer = true;
	}
}






void AEntityScript::actUponCurrentAction(float DeltaTime){
	if(team == teamEnum::enemyTeam){
		if(spottedPlayer && !canSeePlayer){
			actionManager.changeToAction(EActionType::EMoveToPlayer);
		}
	}
	if(team == teamEnum::neutralTeam){
		if(spottedPlayer){
			actionManager.changeToAction(EActionType::EMoveToPlayer);
		}
	}
	
	
	

	EntityAction &currentAction = actionManager.currentAction();
	if(currentAction.actionType() == EActionType::EMoveToPlayer){
		
		//DebugHelper::showScreenMessage("MOVE TO PLAYER");

		moveTowardsPlayer(DeltaTime);
		return;
	}

	if(currentAction.actionType() == EActionType::ERoam){
		return;
	}
	if(currentAction.actionType() == EActionType::EMoveToSpecialPosition){
		if(currentAction.hasTargetPosition() && !hasNodesInPathLeft()){
			FVector targetSpecialPos = currentAction.targetPosition();
			requestNewPathTo(targetSpecialPos, false);
		}
		followpath(DeltaTime);
		return;
	}

}

/// @brief will allow the entity to move towards the player
/// @param deltaTime to calculate the movement speed
void AEntityScript::moveTowardsPlayer(float deltaTime){
	
	if (!hasNodesInPathLeft() && !pathDelayRunning())
	{
		//ask for path
		if(playerPointer != nullptr ){
			FVector target = playerPointer->GetActorLocation();
			requestNewPathTo(target, true);
		}
	}

	//move path
	followpath(deltaTime); 
	
}

void AEntityScript::requestNewPathTo(FVector &targetLocation, bool towardsPlayer){
	UWorld *world = GetWorld();
	if(world != nullptr){
		APathFinder *p = APathFinder::instance();

		//ask for path
		if(p != nullptr){

			FVector a = humanoidPluginController.GetLocation(); 

			this->path = p->getPath(a,targetLocation);

			//no path was found
			if(this->path.size() <= 0){
				DebugHelper::showScreenMessage("Entity path empty", FColor::Yellow);

				resetPathDelay(5.0f); 
				//wait 3 seconds before asking for next path, allows player to move, 
				//better path finding and saving resources because if an issue with the pathfinding occurs,
				//it wont be solved unless the target moves. 
			}
		}
	}
}






/// @brief will allow the entity to follw the path if nodes in path left
/// @param deltaTime for calculating the movement speed
void AEntityScript::followpath(float deltaTime){
	if(hasNodesInPathLeft()){

		DebugHelper::showScreenMessage("Following path.");

		FVector currentLocation = humanoidPluginController.GetLocation(); 

		FVector nextPos = path.front();

		if(reachedPosition(nextPos)){
			path.erase(path.begin() + 0); //first node pop
			
			if(!hasNodesInPathLeft()){
				
				humanoidPluginController.stopLocomotion();
			}
			
			return;
		}else{
			//try to switch to walking state if needed
			humanoidPluginController.setStateWalking();
		}

		if(!canSeePlayer){
			LookAt(nextPos);
		}

		
	}
}

/// @brief clears the path completly
void AEntityScript::resetpath(){
	this->path.clear();
}

/// @brief will return if any nodes are left in the path
/// @return true false
bool AEntityScript::hasNodesInPathLeft(){
	return path.size() > 0;
}

/// @brief will return if a certain position is reached (with some epsilon distance)
/// @param pos position FVector to compare to
/// @return true or false
bool AEntityScript::reachedPosition(FVector pos){
	FVector currentLocation = humanoidPluginController.GetLocation();
	currentLocation.Z = 0;
	pos.Z = 0;

	float dist = FVector::Dist(currentLocation, pos);
	float epsilonDistance = 50.0f;
	return dist < epsilonDistance;
}


/// @brief look at a target
/// @param target 
void AEntityScript::LookAt(AActor *target){
	if(target != nullptr){
		LookAt(target->GetActorLocation());
	}
}



/// @brief look at a location
/// @param TargetLocation target to look at
void AEntityScript::LookAt(FVector TargetLocation) 
{
	/*
    // Calculate the rotation needed to look at the target location
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);

    // Optionally, you can set only the yaw to rotate around the Z-axis
    LookAtRotation.Pitch = 0.0f;
    LookAtRotation.Roll = 0.0f;

    // Apply the rotation to the actor
    SetActorRotation(LookAtRotation);
	*/


	//make humanoid turn
	humanoidPluginController.LookAt(TargetLocation);
}





/// @brief reset the path delay time, allow the player to move to find better path
/// @param time time in seconds to set
void AEntityScript::resetPathDelay(float time){
	pathDelayTimer.Begin(time, false);
}

/// @brief call this mesthod from tick for update
/// @param DeltaTime 
void AEntityScript::updatePathDelay(float DeltaTime){
	pathDelayTimer.Tick(DeltaTime);
}

bool AEntityScript::pathDelayRunning(){
	return !pathDelayTimer.timesUp(); //if not times up: running
}

//-- activate methods --

/// @brief activate or deactivate an enity, --> bool, update and visibilty
/// @param enable true false accordingly
void AEntityScript::enableActiveStatus(bool enable){
	activated = enable;
	enableCollider(enable);
	SetActorHiddenInGame(!enable);
}

/// @brief will return if entity is activated
/// @return true false for tick
bool AEntityScript::isActivatedForUpdate(){
	return activated;
}

/// @brief enable disable collider
/// @param enable true false
void AEntityScript::enableCollider(bool enable){
	SetActorEnableCollision(enable);
}

/// @brief will release the entity to the entity manager
void AEntityScript::die(){
	AlertManager::unSubscribeFromAlert(this);

	
	resetpath();
	enableActiveStatus(false);
	if (EntityManager *e = AworldLevel::entityManager())
	{
		e->add(this);
	}
}

/// @brief despawns the entity
void AEntityScript::despawn(){
	die();
}

/// @brief reduces the spotting time of the entity
void AEntityScript::alert(){
	if(!isActivatedForUpdate()){
		return;
	}
	if(!spottedPlayer){
		updateSpottingTimeOnAlert();
	}
}


void AEntityScript::alert(FVector lookat){
	if (!isActivatedForUpdate())
	{
		return;
	}
	if(!spottedPlayer && !canSeePlayer){
		updateSpottingTimeOnAlert();
		LookAt(lookat);
	}




}

void AEntityScript::updateSpottingTimeOnAlert(){
	spottingTime = defaultSpottingTime / 2.0f; 
	if(spottingTime < spottingTimer.currentTimeLeft()){
		setSpottingTime(spottingTime);
	}
}

void AEntityScript::updateToReducedSpottingTimeIfNotSpottedYet(){
	if(!spottedPlayer){
		updateSpottingTimeOnAlert();
	}
}



/// @brief sets the player spotted status to true immidatly
void AEntityScript::alarm(){
	if(!isActivatedForUpdate()){
		return;
	}
	spottedPlayer = true;
	if(playerPointer != nullptr){
		LookAt(playerPointer->GetActorLocation());
	}
}

void AEntityScript::setTeam(teamEnum teamIn){
	this->team = teamIn;
}

teamEnum AEntityScript::getTeam(){
    return team;
}




//DEBUG
void AEntityScript::drawPath(){
	if(DRAW_PATH){
		if(hasNodesInPathLeft()){
			FVector prev = GetActorLocation();
			for (int i = 0; i < path.size(); i++)
			{
				FVector &current = path[i];
				DebugHelper::showLineBetween(
					GetWorld(),
					prev,
					current,
					FColor::Red,
					0.1f
				);

				prev = current;
			}
		}
	}
}











// ------------ new skelleton section --------------
void AEntityScript::BeginPlayHumanoidController(){
	humanoidPluginController.raycastIgnoreOwner(this);
	humanoidPluginController.defaultSetup(GetWorld());
	humanoidPluginController.setDamagedOwner(this); //must be called after controller setup
}


void AEntityScript::TickHumanoidController(float deltatime){
	humanoidPluginController.Tick(deltatime);
}