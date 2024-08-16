// Fill out your copyright notice in the Description page of Project Settings.

#include "weapon.h"
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "p2/Damageinterface.h"
#include "p2/entityManager/AlertManager.h"
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
	magSkeletonPointer = nullptr;

	// Ensure the World context is valid

	isAiming = false;
	singleFireModeOn = false;

	offset = FVector(-100, 100.0f, 0);

	//time
	timeleft = 0;
	cooldownTime = calculateRpm(600);
	reloadTime = 1.5f;



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
	//showScreenMessage(s);
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

	findAttachmentChildActors();
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
		}else{
			
			//ads and reddot
			if(Type == weaponEnum::assaultRifle){
				pos -= cameraPointer->GetForwardVector().GetSafeNormal() * 25; //20cm
			}
			
			

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


/// @brief Unbind from player or bot
/// will unbind the weapon from the camera or bot Pointer passed when picking up the weapon
void Aweapon::drop(){
	Super::drop();
	/*
	cameraPointer = nullptr;
	botPointer = nullptr; //reset bot too, for both actors designed
	enableCollider(true);
	showWeapon(true);
	*/
	DebugHelper::showScreenMessage("drop weapon");
}

/// @brief releases the shot (mouse up)
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


		int NONE = -1;
		shootProtected(Start, End , NONE);//shoot from a start to an endpoint
	}
}

/// @brief shoot method for the bot
/// will shoot the bot weapon 
/// @param target 
void Aweapon::shootBot(FVector target){
	if(botPointer != nullptr){
		FVector start = botPointer->GetActorLocation();
		FVector connect = (target - start).GetSafeNormal();
		start += connect * 100;

		int NONE = -1;
		shootProtected(start, target, NONE); // protected weapon shoot call
	}
}



/// @brief creates a raycast from start to end point and damages first object within the line
/// IS NOT DESIGNED TO BE CALLED FROM OUT SIDE! ONLY IN CLASS
/// @param Start pos
/// @param End pos target
void Aweapon::shootProtected(FVector Start, FVector End, int ownTeam){
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
		if(botPointer != nullptr){
			Params.AddIgnoredActor(botPointer);
		}

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
					//damage entity if some other team
					int entityTeam = entity->getTeam();
					if(entityTeam != ownTeam){
						DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
						entity->takedamage(20); //must be changed later
					}else{
						//own team hit
						DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
					}

					
				}


			}

			
		}


		//play animation
		shootAnimation();

		float distance = 50000; //50 * 100cm = 50m
		AlertManager::alertInArea(GetWorld(), GetActorLocation(), distance);
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


/// @brief will return if the weapon can perform a reload now
/// @return 
bool Aweapon::canReload(){
	return !isCooling() && isActive();
}

/**
 * reload the weapon with an amount of ammunition. negative numbers are ignored
 */
void Aweapon::reload(int amount){
	if(amount > 0){
		bulletsInMag += amount;
		resetCoolTime(reloadTime);
		reloadAnimation();
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

	FString verschluss_anim_path = "";
	FString mag_anim_path = "";

	//differentiate between the types to set the paths properly for each weapon (type)
	if (Type == weaponEnum::pistol){
		//pistol
		verschluss_anim_path = TEXT("/Game/Prefabs/weapons/pistol/pistolNew/verschlussAnim");
		mag_anim_path = TEXT("/Game/Prefabs/weapons/pistol/pistolMag/magAnim");
	}
	if(Type == weaponEnum::assaultRifle){
		//assault rifle
		verschluss_anim_path = TEXT("/Game/Prefabs/weapons/rifle/verschlussAnim");
		mag_anim_path = TEXT("/Game/Prefabs/weapons/rifle/magAnim");
	}

	 
	setVerschlussPath(verschluss_anim_path);
	setMagAnimPath(mag_anim_path);

}

void Aweapon::setVerschlussPath(FString path){
	verschlussPath = path;
}
void Aweapon::setMagAnimPath(FString path){
	magAnimPath = path;
}

/// @brief setups all components for the animations
void Aweapon::setupAnimations()
{
	animationPathSet();

	FString s;
	// Find all components of type USkeletalMeshComponent attached to this actor

    TArray<USkeletalMeshComponent*> SkeletalMeshComponents; //create t array
    GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents); // ask actor for components

    // Add each component to the output array
    for (USkeletalMeshComponent* Component : SkeletalMeshComponents)
    {
        if (Component)
        {
			FString name = Component->GetName();

			if (name.Contains("verschluss")){
				verschlussSkeletonPointer = Component;
			} else if(name.Contains("mag")){
				magSkeletonPointer = Component;
			}
		}
    }
    
}

/// @brief plays the shoot animation if possible
void Aweapon::shootAnimation(){
	if(verschlussSkeletonPointer != nullptr){
		playAnimation(verschlussPath, verschlussSkeletonPointer, cooldownTime);
	}
}

/// @brief plays the reload animation
void Aweapon::reloadAnimation(){
	if(magSkeletonPointer != nullptr){
		playAnimation(magAnimPath, magSkeletonPointer, reloadTime);
	}
}

/// @brief plays an animatin for a skeleton from a path
/// @param AnimationPath path to the animation
/// @param skeleton 
void Aweapon::playAnimation(
	const FString& AnimationPath, 
	USkeletalMeshComponent *skeleton,
	float time
){


    UAnimSequence* AnimSequence = LoadObject<UAnimSequence>(nullptr, *AnimationPath);
    if (AnimSequence && skeleton){
		float animationLength = AnimSequence->GetPlayLength();
		float playRate = animationLength / time; // cooldownTime; //properly scale

		skeleton->PlayAnimation(AnimSequence, false); // false means don't loop
		// Set the animation speed
        //skeleton->SetPlayRate(60 * cooldownTime);
		skeleton->SetPlayRate(playRate);
	}
}



/// @brief will return a recoil value to apply IF CAN SHOOT
/// @return value (negative) for camera roatation, or 0 if cant shoot at the moment
float Aweapon::recoilValue(){
	if(!canShoot()){
		return 0.0f;
	}
	

	//must be a negative value to properly flip up the camera!
	return -0.1f;
}






/// @brief finds all the attachments in blueprint for the weapon
void Aweapon::findAttachmentChildActors(){
	TArray<UChildActorComponent *> childs;
	GetComponents<UChildActorComponent>(childs);
	if(childs.Num() > 0){
		for (int i = 0; i < childs.Num(); i++){
			if(childs[i] != nullptr){
				FString name = childs[i]->GetName();
				if(name.Contains("reddot")){
					DebugHelper::showScreenMessage("REDDOT FOUND", FColor::Red);

					Super::showChildActor(childs[i], false); //hide reddot test, works!
				}
			}
		}
	}
}