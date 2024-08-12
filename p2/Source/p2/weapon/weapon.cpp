// Fill out your copyright notice in the Description page of Project Settings.

#include "weapon.h"
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "p2/Damageinterface.h"
#include "weaponEnum.h"
#include "carriedItem.h"
#include "p2/playerScript.h"
#include "sightScript.h"

// Sets default values
Aweapon::Aweapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	cameraPointer = nullptr;
	botPointer = nullptr;
	verschlussSkeletonPointer = nullptr;


	// Ensure the World context is valid

	isAiming = false;
	singleFireModeOn = false;

	offset = FVector(-100, 100.0f, 0);

	timeleft = 0;
	cooldownTime = calculateRpm(600);

	bulletsInMag = 30;
}

/// @brief finds the sight component of the weapon if existent
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
            }
        }
    }

    // Log the string to the console
	showScreenMessage(s);
}

/**
 * calculates time to wait for a int of rounds per minute
 */
float Aweapon::calculateRpm(int rpm){
	float rps = rpm / 60;
	return 1 / rps; //1s / rps = wait time in seconds
}

// Called when the game starts or when spawned
void Aweapon::BeginPlay()
{
	Super::BeginPlay();
	setupSight(); //better call in start right
	setupAnimations(); //sets up the animations
	enableCollider(true);
	isVisible = true; //inital setting of visibilty, do not remove!

}

// Called every frame / UPDATE
void Aweapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//followPlayer();
	updateCooltime(DeltaTime);
}


/// @brief Only for player:
/// returns the offset vector of the sight and hipfire by value
/// @return offset vector
FVector Aweapon::getOffsetVector(){
	FVector pos = FVector(0, 0, 0);
	if(cameraPointer != nullptr){
		/*
		if(sightPointer != nullptr){
			//apply height offset
			FVector sightpos = sightPointer->getSightCenter();
			float weaponZ = GetActorLocation().Z;
			float zOffset = sightpos.Z - weaponZ;
			pos.Z = weaponZ;
		}*/

		if(!isAiming){
			//hipfire offset
			pos += cameraPointer->GetRightVector().GetSafeNormal() * 25;
		}	
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
		if(!singleFireMode()){
			abzugHinten = false; //can shoot again if auto release
		}
		
	}
}
/// @brief will say if single fire is on
/// @return true false
bool Aweapon::singleFireMode(){
	if(Type == weaponEnum::pistol){
		return true;
	}
	return singleFireModeOn;
}



/**
 * Unbind from player or bot
 * will unbind the weapon from the camera or bot Pointer passed when picking up the weapon
 */
void Aweapon::dropweapon(){
	cameraPointer = nullptr;
	botPointer = nullptr; //reset bot too, for both actors designed
	enableCollider(true);
	showWeapon(true);
}



void Aweapon::releaseShoot(){
	abzugHinten = false;
}

/// @brief shoot method for player! make sure camera is attached!
void Aweapon::shoot(){
	if(cameraPointer != nullptr && canShoot()){

		FVector ForwardVector = cameraPointer->GetForwardVector();
    	// Now you can use ForwardVector which represents the direction the camera is facing

		// Get the camera location and rotation
    	FVector CameraLocation = cameraPointer->GetComponentLocation();
    	//FRotator CameraRotation = CameraComponent->GetComponentRotation();

		// Define the start and end vectors for the raycast
		FVector Start = CameraLocation + ForwardVector * 100; // todo: owner also by interface! to exclude from query

		FVector End = Start + (ForwardVector * 50000.0f); //50000 units in front of the camera, must be changed later

		shootProtected(Start, End);//shoot from a start to an endpoint
	}
}

/// @brief shoot method for the bot
/// will shoot the bot weapon 
/// @param target 
void Aweapon::shootBot(FVector target){
	if(botPointer != nullptr){
		FVector start = botPointer->GetActorLocation();
		FVector connect = (target - start).GetSafeNormal();
		start += connect * 50;
		shootProtected(start, target); //protected weapon shoot call
	}
}



/// @brief creates a raycast from start to end point and damages first object within the line
/// IS NOT DESIGNED TO BE CALLED FROM OUT SIDE! ONLY IN CLASS
/// @param Start pos
/// @param End pos target
void Aweapon::shootProtected(FVector Start, FVector End){
	//FString::Printf(TEXT("subgraph size %d"), subgraph.size());
	
	if(canShoot()){ //check if can shoot

		abzugHinten = true;

		//showScreenMessage("shoot!");	
		//showScreenMessage("shoot bot 3!");
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


		//play animation
		shootAnimation();
	}
}




/// @brief will check if weapon is able to shoot: mag, cooldown, active state
/// @return bool can shoot right now
bool Aweapon::canShoot(){
	bool single = singleFireMode();
	if(single && abzugHinten){
		return false; //block if is a single fireWeapon
	}

	//here add too for single fire weapons
	return enoughBulletsInMag() && !isCooling() && isActive(); //cant be show if weapon is not selected
}



/// @brief aim method for THE PLAYER -> update each frame.
/// @param aimstatus aim or not 
void Aweapon::aim(bool aimstatus){
	if(Super::isPickedupByPlayer()){
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
	return timeleft > 0.01f;
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

int Aweapon::getBulletsInMag(){
	return bulletsInMag;
}

void Aweapon::showWeapon(bool show){
	isVisible = show;

	SetActorHiddenInGame(!show);  // Hides the actor if 'show' is false
}




////p2/Content/Prefabs/Weapons/pistol/pistolAnimated/verschlussAnim.uasset

/// @brief will set the paths to the animations based on weapon enum type
/// to not create subclasses! DO NOT REMOVE
void Aweapon::animationPathSet(){
	//TEXT("/Game/Prefabs/weapons/pistol/pistolNew/verschlussAnim")

	FString verschluss_path = "";

	//differentiate between the types to set the paths properly for each weapon (type)
	if (Type == weaponEnum::pistol){
		//pistol
		verschluss_path = TEXT("/Game/Prefabs/weapons/pistol/pistolNew/verschlussAnim");
	}
	if(Type == weaponEnum::assaultRifle){
		//assault rifle
	}

	 
	setVerschlussPath(verschluss_path);
}

void Aweapon::setVerschlussPath(FString path){
	verschlussPath = path;
}

/// @brief setups all components for the animations
void Aweapon::setupAnimations()
{
	animationPathSet();

	FString s;
	// Find all components of type USkeletalMeshComponent attached to this actor
    TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
    GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);

    // Add each component to the output array
    for (USkeletalMeshComponent* Component : SkeletalMeshComponents)
    {
        if (Component)
        {
			FString name = Component->GetName();
            s.Append(TEXT("Child: "));
			s.Append(name);
			s.Append("\n");

			if (name.Contains("verschluss"))
			{
				verschlussSkeletonPointer = Component;
			}
        }
    }
	// Log the string to the console
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, s);
    }
    
}

/// @brief plays the shoot animation if possible
void Aweapon::shootAnimation(){
	if(verschlussSkeletonPointer != nullptr){
		playAnimation(verschlussPath, verschlussSkeletonPointer);
	}
}

/// @brief plays an animatin for a skeleton from a path
/// @param AnimationPath path to the animation
/// @param skeleton 
void Aweapon::playAnimation(const FString& AnimationPath, USkeletalMeshComponent *skeleton){
    UAnimSequence* AnimSequence = LoadObject<UAnimSequence>(nullptr, *AnimationPath);
    if (AnimSequence && skeleton){
		float animationLength = AnimSequence->GetPlayLength();
		float playRate = animationLength / cooldownTime; //properly scale

		skeleton->PlayAnimation(AnimSequence, false); // false means don't loop
		// Set the animation speed
        //skeleton->SetPlayRate(60 * cooldownTime);
		skeleton->SetPlayRate(playRate);
	}
}