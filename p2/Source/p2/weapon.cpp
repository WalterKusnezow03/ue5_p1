// Fill out your copyright notice in the Description page of Project Settings.

#include "weapon.h"
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "Damageinterface.h"
#include "sightScript.h"

// Sets default values
Aweapon::Aweapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	cameraPointer = nullptr;
	// Ensure the World context is valid

	isAiming = false;

	offset = FVector(-100, 100.0f, 0);

	timeleft = 0;
	cooldownTime = calculateRpm(700);

	bulletsInMag = 30;
}


void Aweapon::setupSight(){
	TArray<AActor*> ChildActors;
    FString s;
    GetAllChildActors(ChildActors, true);
    for (AActor* Child : ChildActors)
    {
        if (Child)
        {
            FString ChildName = Child->GetName();
            FString ChildType = Child->GetClass()->GetName();

            s.Append(ChildName);
            s.Append(TEXT(" - "));
            s.Append(ChildType);
            s.Append(TEXT("\n"));

            // Check if the child is of type AsightScript
            AsightScript* SightChild = Cast<AsightScript>(Child);
            if (SightChild){
				sightPointer = SightChild;

				s.Append(TEXT(" (AsightScript)"));
                UE_LOG(LogTemp, Warning, TEXT("FOUND CHILDS NEW: %s (AsightScript)"), *SightChild->GetName());
            }
            else{
                UE_LOG(LogTemp, Warning, TEXT("FOUND CHILDS NEW: %s (%s)"), *ChildName, *ChildType);
            }
        }
    }

    // Log the string to the console
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, s);
    }

    // Alternatively, you can use UE_LOG to log to the output log
    UE_LOG(LogTemp, Warning, TEXT("FOUND CHILDS NEW: %s"), *s);
}


/**
 * shows a screen message for debugging
 */
void Aweapon::showScreenMessage(FString s){
	if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, s);
    }
}

/**
 * calculates time to wait for a int of rounds per minute
 */
float Aweapon::calculateRpm(int rpm){
	float rps = rpm / 60;
	return 1 / rps; //1s / rps wait time
}

// Called when the game starts or when spawned
void Aweapon::BeginPlay()
{
	Super::BeginPlay();
	setupSight(); //better call in start right
	enableCollider(true);
}

// Called every frame / UPDATE
void Aweapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	followPlayer();
	updateCooltime(DeltaTime);
}

/**
 * will follow the player or entity if is picked up by the player
 */
void Aweapon::followPlayer(){
	
	if (isPickedup())
	{
		FVector targetPos = cameraPointer->GetComponentLocation() + 
							cameraPointer->GetForwardVector() * 100.0f +
							getOffsetVector();

        FRotator targetRotation = cameraPointer->GetComponentRotation();
        
        // Smoothly interpolate position and rotation
        FVector currentPos = GetActorLocation();
        FRotator currentRotation = GetActorRotation();
        
        SetActorLocation(FMath::VInterpTo(currentPos, targetPos, GetWorld()->GetDeltaSeconds(), 50.0f));
        SetActorRotation(FMath::RInterpTo(currentRotation, targetRotation, GetWorld()->GetDeltaSeconds(), 50.0f));
	}
}

/**
 * returns the offset vector of the sight by value
 */
FVector Aweapon::getOffsetVector(){
	
	FVector pos = FVector(0, 0, 0);

	/*
	if(sightPointer != nullptr){
		
		FVector sightpos = sightPointer->getSightCenter();
		float weaponZ = GetActorLocation().Z;
		float zOffset = sightpos.Z - weaponZ;
		pos.Z = weaponZ;
		
		//showScreenMessage()
	}*/

	if(!isAiming){
		//hipfire offset
		pos += cameraPointer->GetRightVector().GetSafeNormal() * 100;
	}

	return pos;
}



/**
 * will update the cooldown time / decrease it
 */
void Aweapon::updateCooltime(float time){
	if(isCooling()){
		timeleft -= time;
	}else{
		abzugHinten = false;
	}
}



/// @brief will enable and disable the collider for the actor
/// @param enable 
void Aweapon::enableCollider(bool enable){
	SetActorEnableCollision(enable);
}



/**
 * allows the player to pickup the weapon
 */
void Aweapon::pickup(UCameraComponent &cameraRefIn){
	if(!isPickedup()){
		cameraPointer = &cameraRefIn; // Assign the address of cameraRefIn to cameraRef
		enableCollider(false);
	}
}


/**
 * will unbind the weapon from the camera Pointer passed when picking up the weapon
 */
void Aweapon::dropweapon(){
	cameraPointer = nullptr;
	enableCollider(true);
	showWeapon(true);
}

/**
 * returns a boolean if its picked up or not
 */
bool Aweapon::isPickedup(){
	return cameraPointer != nullptr;
}





void Aweapon::releaseShoot(){
	abzugHinten = false;
}

// with default camera which is passed in by then
void Aweapon::shoot(){
	if(isPickedup() && canShoot()){
		abzugHinten = true;

		FVector ForwardVector = cameraPointer->GetForwardVector();
    	// Now you can use ForwardVector which represents the direction the camera is facing

		// Get the camera location and rotation
    	FVector CameraLocation = cameraPointer->GetComponentLocation();
    	//FRotator CameraRotation = CameraComponent->GetComponentRotation();

		// Define the start and end vectors for the raycast
		FVector Start = CameraLocation + ForwardVector * 100; // todo: owner also by interface! to exclude from query

		FVector End = Start + (ForwardVector * 50000.0f); //50000 units in front of the camera, must be changed later

		shoot(Start, End);//shoot from a start to an endpoint
	}
} 


/**
 * creates a raycast from start to end point and damages first object within the line
 */
void Aweapon::shoot(FVector Start, FVector End){
	if(isPickedup() && canShoot()){
		//showScreenMessage("shoot!");

		//FVector direction = (to - from).GetSafeNormal(); // AB = B - A
		resetCoolTime(cooldownTime);
		//timeleft = cooldownTime; // reset time

		// Perform the raycast
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // Ignore the character itself

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);


		// If the raycast hit something, log the hit actor's name
		if (bHit)
		{
			AActor* actor = HitResult.GetActor();
			if(actor != nullptr){
				
				//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *actor->GetName()); // dereference and call method

				IDamageinterface* entity = Cast<IDamageinterface>(actor);
				if (entity)
				{
					DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
					entity->takedamage(20); //must be changed later
				}


			}

			
		}
	
	}
}



bool Aweapon::canShoot(){
	//here add too for single fire weapons

	return enoughBulletsInMag() && !isCooling() && isActive(); //cant be show if weapon is not selected
}

/**
 * aim the weapon
 */
void Aweapon::aim(bool aimstatus){
	if(isPickedup()){
		isAiming = aimstatus;

		showScreenMessage("switch");
	}
}



void Aweapon::resetCoolTime(float time){
	timeleft = time;
}

/**
 * will return if the weapon is cooling at the moment
 */
bool Aweapon::isCooling(){
	return timeleft > 0;
}

/**
 * will return if the weapon is cooling at the moment
 */
bool Aweapon::enoughBulletsInMag(){
	return bulletsInMag > 0;
}


/**
 * reload the weapon with an amount of ammunition. negative numbers are ignored
 */
void Aweapon::reload(int amount){
	if(amount > 0){
		bulletsInMag += amount;
	}
}



int Aweapon::getMagSize(){
	return 30;
}



void Aweapon::showWeapon(bool show){
	isVisible = show;

	SetActorHiddenInGame(!show);  // Hides the actor if 'show' is false
}


/// @brief returns is active status for player (hidden or not)
/// @return is active (visible status)
bool Aweapon::isActive(){
	return isVisible;
}