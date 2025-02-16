// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/player/playerScript.h"
#include "p2/pathFinding/PathFinder.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
#include "Kismet/KismetMathLibrary.h"
#include "p2/DebugHelper.h"
#include "p2/player/teamEnum.h"
#include "EntityScript.h"

// Sets default values
AEntityScript::AEntityScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AEntityScript::BeginPlay()
{
	Super::BeginPlay();
	init();
	setupBoneController();
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


    spottedPlayer = false;
    canSeePlayer = false;


	//set team
	//setTeam(referenceManager::TEAM_NEUTRAL);
	setTeam(teamEnum::neutralTeam);


	FVector offset = GetActorLocation();
	boneController.SetLocation(offset);

	
}

void AEntityScript::setupBoneController(){
	

	//init offset for now

	FVector offset = GetActorLocation();
	boneController.SetLocation(offset);

	// debug testing meshes
	float legScaleCM = 100.0f;
	float armScaleCM = 70.0f;
	float legHalfScale = legScaleCM / 2.0f;
	float armHalfScale = armScaleCM / 2.0f;

	int sizeX = 10;
	int sizeY = 10;
	int offY = sizeY / 2;
	offY = 0;

	AActor *oberschenkel = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	AActor *unterschenkel = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	boneController.attachLimbMeshes(oberschenkel, unterschenkel, 1); //foot 1 debug
	
	AActor *oberschenkel_1 = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	AActor *unterschenkel_1 = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	boneController.attachLimbMeshes(oberschenkel_1, unterschenkel_1, 2); //foot 2 debug

	
	AActor *oberarm = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	AActor *unterarm = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	boneController.attachLimbMeshes(oberarm, unterarm, 3); //hand 1 debug
	

	//torso
	/**
	 * torso wird jetzt erstmal auch hier erstellt
	 * 
	 * die create limb methode usw muss irgendwann entweder durch meshes
	 * aus den assets ersetzt werden
	 * oder eine eigene klasse existieren die diese detailierter
	 * erstellen kann!
	 */
	AActor *torsoMesh = createLimbPivotAtTop(sizeX, sizeY * 4, -armScaleCM, 0);
	boneController.attachTorso(torsoMesh);


	//holding weapon
	AActor *oberarm_1 = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	AActor *unterarm_1 = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	boneController.attachLimbMeshes(oberarm_1, unterarm_1, 4); //hand 2 debug
	
	//foot
	AActor *foot1 = createLimbPivotAtTop(20, 10, 10, 10);
	AActor *foot2 = createLimbPivotAtTop(20, 10, 10, 10);
	boneController.attachPedalFoots(foot1, foot2);


	//head
	AActor *headPointer = createLimbPivotAtTop(15, 20, -1 * 25, 0); //-35 flip pivot
	boneController.attachHead(headPointer);


	

	//DEBUG HIDE OWN MESH 
	
}


AActor *AEntityScript::createLimbPivotAtTop(int x, int y, int height, int pushFront){

	height *= -1; //orient downwardss
	/**
	 * DEBUG CREATE FOLLOW LIMBS
	 */
	UMaterial *material = nullptr;
	assetManager *assetManagerPointer = assetManager::instance();
	if(assetManagerPointer != nullptr){
		material = assetManagerPointer->findMaterial(materialEnum::wallMaterial);
	}

	EntityManager *entitymanagerPointer = worldLevel::entityManager();
	if(entitymanagerPointer != nullptr){
		FVector location(0, 0, 0);
		AcustomMeshActor *oberschenkel = entitymanagerPointer->spawnAcustomMeshActor(GetWorld(), location);
		if(oberschenkel != nullptr){
			//int width = 10;
			//int height = -(legScaleCM / 2);

			float xHalf = x / 2.0f;
			float yHalf = y / 2.0f;

			FVector a(-xHalf + pushFront, -yHalf,0);
			FVector b(xHalf + pushFront, -yHalf, 0);
			FVector c(xHalf + pushFront, yHalf,0);
			FVector d(pushFront, yHalf,0);


			FVector at(-xHalf + pushFront, -yHalf, height);
			FVector bt(xHalf + pushFront, -yHalf, height);
			FVector ct(xHalf + pushFront, yHalf, height);
			FVector dt(pushFront, yHalf, height);

			oberschenkel->createCube(
				a,b,c,d,at,bt,ct,dt,
				material
			);

			entitymanagerPointer->addActorToIgnoreRaycastParams(
				this, getTeam()
			);

			oberschenkel->setDamagedOwner(this);

			return oberschenkel;
		}
	}
	return nullptr;
}










// Called every frame
void AEntityScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//only update if activated
	if(!isActivatedForUpdate()){
		return;
	}

	
	//tick bone controller
	if(true){
		boneController.Tick(DeltaTime, GetWorld());
		SetActorLocation(boneController.GetLocation());
	}
	


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
	
	bool withinAngle = withinVisionAngle(playerPointer);
	bool withinRange = isWithinMaxRange(playerPointer->GetActorLocation());

	
	// if not spotted yet, check angle, if angle ok, check vision
	if(withinAngle && withinRange){
		canSeePlayer = performRaycast(playerPointer);
		if(canSeePlayer){
			//DebugHelper::showScreenMessage("player vision check", FColor::Red);
		}
	}


	//look at player when spotted and within angle
	/*
	if(canSeePlayer && spottedPlayer && withinAngle){
		LookAt(playerPointer);
	}
	*/

	//act based on vision
	if(canSeePlayer){
		if(!spottedPlayer){
			updateSpottingTime(DeltaTime);
		}
	}else{
		//cant see player
		if(!spottedPlayer){
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

	//showScreenMessage("enemy entity damage");
	health -= d;
	if(health <= 0){
		d = 0;
		die();
	}
}
/// @brief grenade damage is not specified here directly, will only damage as default
/// @param d 
/// @param from 
void AEntityScript::takedamage(int d, FVector &from){
	takedamage(d);
}

/// @brief checks if an actor is within 180 degree range to own forward vector
/// @param target 
bool AEntityScript::withinVisionAngle(AActor *target){
	if(target != nullptr){

		//wenn das skalarpdoukt zweier vektoren 0 ergibt sind sie orthogonal zu einander
		//wenn das skalarprodukt zweier vektoren 1 ergibt sind sie paralell zu einander

		//FVector forward = GetActorForwardVector().GetSafeNormal();
		FVector forward = boneController.lookDirection();
		FVector currentLocation = boneController.GetLocation();

		//ab = b - a
		FVector ab = (target->GetActorLocation() - currentLocation).GetSafeNormal();

		float skalarprodukt = FVector::DotProduct(forward, ab);


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
	FVector currentLocation = boneController.GetLocation(); //GetActorLocation()
	return (FVector::Dist(currentLocation, vec) <= MAXDISTANCE);
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


		if(EntityManager *e = worldLevel::entityManager()){
			//ignoreParams = e->getIgnoredRaycastParams(); //example for getting all
			ignoreParams = e->getIgnoredRaycastParams(getTeam());
		}

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, ignoreParams);
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
bool AEntityScript::performRaycast(FVector &direction, FVector &output, int cmLength) //because a reference is expected it must be valid
{
	
	// Define the start and end vectors for the raycast
	//FVector Start = this->GetActorLocation();

	FVector Start = boneController.GetLocation();
	direction = direction.GetSafeNormal();
	Start += direction * 100; //50cm

	// Get the camera location and rotation
	FVector End = Start + direction * cmLength;

	// Perform the raycast
	FHitResult HitResult;

	if (EntityManager *e = worldLevel::entityManager())
	{
		// ignoreParams = e->getIgnoredRaycastParams(); //example for getting all
		ignoreParams = e->getIgnoredRaycastParams();
	}

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, ignoreParams);
	
	// If the raycast hit something, log the hit actor's name
	if (bHit)
	{
		output = HitResult.ImpactPoint;
	}

	return bHit;
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
	if(spottingTimeLeft > 0.0f){
		spottingTimeLeft -= deltaTime;
	}else{
		spottingTimeLeft = 0.0f;
		spottedPlayer = true;
	}
}




/// @brief will allow the entity to move towards the player
/// @param deltaTime to calculate the movement speed
void AEntityScript::moveTowardsPlayer(float deltaTime){
	if(spottedPlayer && !canSeePlayer){
		if (!hasNodesInPathLeft() && !pathDelayRunning())
		{
			//ask for path
			UWorld *world = GetWorld();
			if(world != nullptr){
				//get or create pathfinder
				PathFinder *p = PathFinder::instance(world);

				//ask for path
				if(p != nullptr && playerPointer != nullptr ){

					

					FVector a = GetActorLocation();

					a = boneController.GetLocation();
					FVector b = playerPointer->GetActorLocation();
					this->path = p->getPath(a,b);

					//no path was found
					if(this->path.size() <= 0){
						DebugHelper::showScreenMessage("Entity path empty", FColor::Yellow);

						resetPathDelay(3.0f); //wait 3 seconds before asking for next path, allows player to move, 
						//better path finding and saving resources because if an issue with the pathfinding occurs,
						//it wont be solved unless the target moves. 
					}
				}
			}
		}

		//move path
		DebugHelper::showScreenMessage("move to player path!");
		followpath(deltaTime); //testing
	}
}

/// @brief will allow the entity to follw the path if nodes in path left
/// @param deltaTime for calculating the movement speed
void AEntityScript::followpath(float deltaTime){
	if(hasNodesInPathLeft()){
		//float speed = 350.0f; //3m/s

		

		//FVector currentLocation = GetActorLocation();

		FVector currentLocation = boneController.GetLocation();
		FVector nextPos = path.front();

		if(reachedPosition(nextPos)){
			path.erase(path.begin() + 0); //first node pop
			
			if(!hasNodesInPathLeft()){
				boneController.stopLocomotion();
			}
			
			return;
		}else{
			//try to switch to walking state if needed
			boneController.setStateWalking();
		}


		// Direction vector from current location to target location
		//FVector dir = (nextPos - currentLocation).GetSafeNormal(); // Normalize the direction vector


		//x(t) = pos + velocity * dt + 1/2 (accelrattion * dt)^2
		//x(t) = x0 + v0t + 1/2 at^2
		//x(t) = x0 + v * speed * deltaTime 
		// Move from current location towards target location by stepDistance
		//FVector newLocation = currentLocation + dir * deltaTime * speed;
		//SetActorLocation(newLocation);



		//NEW BONE CONTROLLER INTERACTION!
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
	FVector s = GetActorLocation();

	s = boneController.GetLocation();
	float dist = FVector::Dist(s, pos);
	float epsilonDistance = 100;
	if (dist < epsilonDistance){
		return true;
	}

	s.Z = 0;
	pos.Z = 0;
	dist = FVector::Dist(s, pos);
	if (dist < epsilonDistance){
		return true;
	}


	//2D distance
	s.Z = 0;
	pos.Z = 0;
	return FVector::Dist(s, pos) < epsilonDistance;

	//return false;
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
    // Calculate the rotation needed to look at the target location
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);

    // Optionally, you can set only the yaw to rotate around the Z-axis
    LookAtRotation.Pitch = 0.0f;
    LookAtRotation.Roll = 0.0f;

    // Apply the rotation to the actor
    SetActorRotation(LookAtRotation);


	//NEW
	boneController.LookAt(TargetLocation);
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
	resetpath();
	if (EntityManager *e = worldLevel::entityManager())
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

void AEntityScript::setTeam(teamEnum teamIn){
	this->team = teamIn;
	// referenceManager::verifyTeam(teamIn);
}

teamEnum AEntityScript::getTeam(){
    return team;
}