// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/playerScript.h"
#include "p2/pathFinding/PathFinder.h"
#include "p2/referenceManager.h"
#include "p2/entityManager/EntityManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "p2/DebugHelper.h"
#include "EntityScript.h"

// Sets default values
AEntityScript::AEntityScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	health = 100;
	spottedPlayer = false;
	playerPointer = nullptr;
	defaultSpottingTime = 5;
	setSpottingTime(defaultSpottingTime);
}

// Called when the game starts or when spawned
void AEntityScript::BeginPlay()
{
	Super::BeginPlay();
	init();
}

/// @brief will enable the entity for tick
/// set the player pointer, 
/// set the spotting time and
/// the entity team
void AEntityScript::init(){
	//DebugHelper::showScreenMessage("entity init");
	enableActiveStatus(true);

	health = 100;
	spottedPlayer = false;
	playerPointer = nullptr;
	defaultSpottingTime = 5;
	setSpottingTime(defaultSpottingTime);
	setupRaycastIgnoreParams();


	//set team
	setTeam(referenceManager::TEAM_NEUTRAL);

}

// Called every frame
void AEntityScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//only update if activated
	if(!isActivatedForUpdate()){
		return;
	}

	//get player pointer if needed
	if(playerPointer == nullptr){
		referenceManager *i = referenceManager::instance();
		if(i){
			playerPointer = i->getPlayerPointer();
		}
	}

	//rest of update
	canSeePlayer = false; //reset
	bool withinAngle = withinVisionAngle(playerPointer);
	bool withinRange = isWithinMaxRange(playerPointer->GetActorLocation());

	//if not spotted yet, check angle, if angle ok, check vision
	if(withinAngle && withinRange){
		canSeePlayer = performRaycast(playerPointer);
	}

	//look at player when spotted and within angle
	if(spottedPlayer && withinAngle){
		LookAt(playerPointer);
	}

	//act based on vision
	if(canSeePlayer){

		if(spottedPlayer){
			//attack if can see and spotted
			//showScreenMessage("spotted player");

		}else{
			//update time if can see, but not spotted
			updateSpottingTime(DeltaTime);
		}


	}else{
		//cant see player
		if(spottedPlayer){
			//follow
		}else{
			//cant see, hasnt spotted: reset
			setSpottingTime(defaultSpottingTime);
		}
	}


	//moves towards player is spotted and cant see player
	moveTowardsPlayer(DeltaTime);

	updatePathDelay(DeltaTime);
}

//allows the entity to take damage
void AEntityScript::takedamage(int d){

	showScreenMessage("enemy entity damage");
	health -= d;
	if(health <= 0){
		d = 0;
		die();
	}
}




/// @brief checks if an actor is within 180 degree range to own forward vector
/// @param target 
bool AEntityScript::withinVisionAngle(AActor *target){
	if(target != nullptr){

		//wenn das skalarpdoukt zweier vektoren 0 ergibt sind sie orthogonal zu einander
		//wenn das skalarprodukt zweier vektoren 1 ergibt sind sie paralell zu einander

		FVector forward = GetActorForwardVector().GetSafeNormal();
		//ab = b - a
		FVector ab = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		float skalarprodukt = FVector::DotProduct(forward, ab);

		

		//mindestens orthogonal oder näher an der 1
		if(skalarprodukt >= 0){
			//showScreenMessage("angle ok");
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
	return (FVector::Dist(GetActorLocation(), vec) <= MAXDISTANCE);
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

		//FVector dir = (End - Start);

		//End = Start + dir * 1.1f; //safety reasons
		

		// Perform the raycast
		FHitResult HitResult;


		/*
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // Ignore the character itself
		// Iterate and ignore all child actors
		TArray<AActor*> ChildActors;
		this->GetAttachedActors(ChildActors);

		for (AActor* ChildActor : ChildActors)
		{
			Params.AddIgnoredActor(ChildActor);
		}*/


		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, ignoreParams);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);



		// If the raycast hit something, log the hit actor's name
		if (bHit)
		{
			//showScreenMessage("hit raycast");
			AActor *actor = HitResult.GetActor();
			if(actor == playerPointer){
				return true;
			}

			if(actor == target){
				return true;
			}

			
		}
	}
	return false;
	
}

/**
 * sets the spotting time a given value
 */
void AEntityScript::setSpottingTime(float time){
	spottingTimeLeft = time;
}

/**
 * update the spotting time
 */
void AEntityScript::updateSpottingTime(float deltaTime){
	if(spottingTimeLeft > 0){
		spottingTimeLeft -= deltaTime;
	}else{
		spottingTimeLeft = 0;
		spottedPlayer = true;
	}
}




/// @brief will allow the entity to move towards the player
/// @param deltaTime to calculate the movement speed
void AEntityScript::moveTowardsPlayer(float deltaTime){
	if(spottedPlayer && !canSeePlayer){
		if(!hasNodesInPathLeft()){
			//ask for path
			UWorld *world = GetWorld();
			if(world != nullptr){
				//get or create pathfinder
				PathFinder *p = PathFinder::instance(world);

				//ask for path
				if(p != nullptr && playerPointer != nullptr && !pathDelayRunning()){
					FVector a = GetActorLocation();
					FVector b = playerPointer->GetActorLocation();
					this->path = p->getPath(a,b);

					//no path was found
					if(this->path.size() <= 0){
						resetPathDelay(3.0f); //wait 3 seconds before asking for next path, allows player to move, 
						//better path finding and saving resources because if an issue with the pathfinding occurs,
						//it wont be solved unless the target moves. 
					}
				}
			}
			
		}

		//move path
		followpath(deltaTime); //testing
	}
}

/// @brief will allow the entity to follw the path if nodes in path left
/// @param deltaTime for calculating the movement speed
void AEntityScript::followpath(float deltaTime){
	if(hasNodesInPathLeft()){
		float speed = 300.0f; //3m/s

		
		// showScreenMessage("moving");

		FVector currentLocation = GetActorLocation();
		FVector nextPos = path.front();

		if(reachedPosition(nextPos)){
			path.erase(path.begin() + 0); //first node pop
			return;
		}

		//showScreenMessage(FString::Printf(TEXT("pos %d, %d"), currentLocation.X, currentLocation.Y));

		// Direction vector from current location to target location
		FVector dir = (nextPos - currentLocation).GetSafeNormal(); // Normalize the direction vector


		//x(t) = x0 + v0t + 1/2 at^2
		//x(t) = x0 + v * speed * deltaTime 
		// Move from current location towards target location by stepDistance
		FVector newLocation = currentLocation + dir * deltaTime * speed; 

		SetActorLocation(newLocation);
	}
}

void AEntityScript::resetpath(){

}

/// @brief will return if any nodes are left in the path
/// @return true false
bool AEntityScript::hasNodesInPathLeft(){
	return !path.empty();
}

/// @brief will return if a certain position is reached (with some epsilon distance)
/// @param pos position FVector to compare to
/// @return true or false
bool AEntityScript::reachedPosition(FVector pos){
	FVector s = GetActorLocation();
	float dist = FVector::Dist(s, pos);
	float epsilonDistance = 25;
	if (dist < epsilonDistance){
		return true;
	}
	return false;
}


/// @brief look at a target
/// @param target 
void AEntityScript::LookAt(AActor *target){
	if(target != nullptr){
		LookAt(target->GetActorLocation());
	}
}


//hier ist zum beispiel apss by value weil referenz einfach kein sinn macht
//für so einen kurzen block
/// @brief look at a location
/// @param TargetLocation target to look at
void AEntityScript::LookAt(FVector TargetLocation) 
{
    // Calculate the rotation needed to look at the target location
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);

    // Optionally, you can set only the yaw to rotate around the Z-axis
    LookAtRotation.Pitch = 0.0f;
    LookAtRotation.Roll = 0.0f;

    // Apply the rotation to the actor
    SetActorRotation(LookAtRotation);
}






void AEntityScript::showScreenMessage(FString s){
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, s);
	}
}




/// @brief reset the path delay time, allow the player to move to find better path
/// @param time time in seconds to set
void AEntityScript::resetPathDelay(float time){
	pathDelay = time;
}

/// @brief call this mesthod from tick for update
/// @param DeltaTime 
void AEntityScript::updatePathDelay(float DeltaTime){
	if(pathDelayRunning()){
		pathDelay -= DeltaTime;
	}
}

/// @brief returns if the delay is still running
/// @return 
bool AEntityScript::pathDelayRunning(){
	return pathDelay > 0.01f;
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

	if(EntityManager *e = EntityManager::instance()){
		e->add(this);
	}
}

/// @brief despawns the entity
void AEntityScript::despawn(){
	die();
}

/// @brief reduces the spotting time of the entity
void AEntityScript::alert(){
	if(!spottedPlayer){
		defaultSpottingTime /= 2;

		//update time if lower
		if(defaultSpottingTime < spottingTimeLeft){
			setSpottingTime(defaultSpottingTime);
		}
	}
}


void AEntityScript::alert(FVector lookat){
	if(!spottedPlayer && !canSeePlayer){
		defaultSpottingTime /= 2; 
		setSpottingTime(defaultSpottingTime);
		LookAt(lookat);
	}
}

/// @brief sets the player spotted status to true immidatly
void AEntityScript::alarm(){
	spottedPlayer = true;
	if(playerPointer != nullptr){
		LookAt(playerPointer->GetActorLocation());
	}
}

void AEntityScript::setTeam(int teamIn){
    this->team = referenceManager::verifyTeam(teamIn);
}

int AEntityScript::getTeam(){
    return team;
}