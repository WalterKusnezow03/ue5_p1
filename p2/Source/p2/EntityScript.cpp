// Fill out your copyright notice in the Description page of Project Settings.

#include "playerScript.h"
#include "PathFinder.h"
#include "referenceManager.h"
#include "Kismet/KismetMathLibrary.h"
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
	health = 100;
	spottedPlayer = false;
	playerPointer = nullptr;
	defaultSpottingTime = 5;
	setSpottingTime(defaultSpottingTime);


	//testing weapons
	referenceManager *instance = referenceManager::instance();
	if(instance != nullptr){
		Aweapon *w = instance->spawnAweapon(GetWorld());
		showScreenMessage("begin weapon");
		if (w != nullptr){
			showScreenMessage("pickup weapon");
			w->pickupBot(this);
		}
	}
}

// Called every frame
void AEntityScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//get player pointer if needed
	if(playerPointer == nullptr){
		referenceManager *i = referenceManager::instance();
		if(i){
			playerPointer = i->getPlayerPointer();
		}

		
	}

	//rest of update
	bool canSeePlayer = false;
	bool withinAngle = withinVisionAngle(playerPointer);

	//if not spotted yet, check angle, if angle ok, check vision
	if(!spottedPlayer && withinAngle){
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
		if(spottedPlayer){
			//follow
		}else{
			//cant see, hasnt spotted: reset
			setSpottingTime(defaultSpottingTime);
		}
	}


	//moves towards player is spotted and cant see player
	moveTowardsPlayer(DeltaTime, canSeePlayer);
	
}


//allows the entity to take damage
void AEntityScript::takedamage(int d){

	showScreenMessage("entity damage");
	health -= d;
	if(health <= 0){
		d = 0;
		die();
	}
}


void AEntityScript::die(){

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


/// @brief performs a raycast to the target and checks if "can see it"
/// @param target aactor from the scene
/// @return can see or not as bool
bool AEntityScript::performRaycast(AActor *target) //because a reference is expected it must be valid
{
	
	if (target)
	{
		
		// Get the camera location and rotation
		FVector End = target->GetActorLocation();

		// Define the start and end vectors for the raycast
		FVector Start = this->GetActorLocation();

		End = Start + (End - Start) * 1.1f; //safety reasons

		// Perform the raycast
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // Ignore the character itself

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);



		// If the raycast hit something, log the hit actor's name
		if (bHit)
		{
			//showScreenMessage("hit raycast");
			AActor *actor = HitResult.GetActor();
			if(actor == playerPointer){
				return true;
			}

			if(actor != nullptr){
				
				//die POINTER vergleichen, einmal den pointer, einmal die &adresse
				if(actor == target){
					return true;
				}
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












void AEntityScript::moveTowardsPlayer(float deltaTime, bool canSeePlayer){
	if(spottedPlayer && !canSeePlayer){
		if(!hasNodesInPathLeft()){
			//ask for path
			UWorld *world = GetWorld();
			if(world != nullptr){
				//get or create pathfinder
				PathFinder *p = PathFinder::instance(world);

				//ask for path
				if(p != nullptr && playerPointer != nullptr ){
					FVector a = GetActorLocation();
					FVector b = playerPointer->GetActorLocation();
					this->path = p->getPath(a,b);
				}
			}
			
		}

		//move path
		followpath(deltaTime); //testing
	}
}

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

bool AEntityScript::hasNodesInPathLeft(){
	return !path.empty();
}


bool AEntityScript::reachedPosition(FVector pos){
	FVector s = GetActorLocation();
	float dist = FVector::Dist(s, pos);
	float oneMeter = 50;
	if (dist < oneMeter){
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



/// @brief look at a location
/// @param TargetLocation 
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
