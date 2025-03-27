// Fill out your copyright notice in the Description page of Project Settings.

#include "weapon.h"
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "p2/interfaces/Damageinterface.h"
#include "p2/entityManager/AlertManager.h"
#include "weaponEnum.h"
#include "carriedItem.h"
#include "p2/player/playerScript.h"
#include "sightScript.h"
#include "ammunitionEnum.h"
#include <map>
#include "p2/entities/customIk/MMatrix.h"
#include "p2/player/teamEnum.h"
#include "p2/entities/HumanEntityScript.h"
#include "p2/util/AActorUtil.h"
#include "p2/DebugHelper.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/gamestart/assetManager.h"
#include "p2/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "carriedItem.h"


// Sets default values
Aweapon::Aweapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	cameraPointer = nullptr;
	botPointer = nullptr;
	verschlussSkeletonPointer = nullptr;
	magSkeletonPointer = nullptr;

	rightHandTargetSkelletonPointer = nullptr;
	leftHandTargetSkelletonPointer = nullptr;

	reddotSightChildActor = nullptr;
	ironSightChildActor = nullptr;

	// Ensure the World context is valid
	isAiming = false;
	singleFireModeOn = false;

	offset = FVector(-100, 100.0f, 0);

	//time
	
	cooldownTime = calculateRpm(600);
	reloadTime = 1.5f;


}


void Aweapon::resetFlags(){
	kickbackStarted = false;
	verschlussKickBackStarted = false;
	recoilCopied = false;
	abzugHinten = false; 
	isReloading = false;
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
	
	setupAnimations(); //sets up the animations
	enableCollider(true);
	isVisible = true; //inital setting of visibilty, do not remove!

	findAttachmentChildActors(); //DEPRECATED

	//only on begin play!
	spawnAllAvailableAttachments();

	setupKickBackAnimation();
	setupVerschlussAnimation();



	bulletsInMag = getMagSize();
}

// Called every frame / UPDATE
void Aweapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//followPlayer();
	updateCooltime(DeltaTime);

	//new
	TickKickback(DeltaTime);
	TickVerschlussKickBack(DeltaTime);
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
	//new
	if(!timer.timesUp()){
		timer.Tick(time);
	}else{
		if(!singleFireMode()){
			abzugHinten = false; 
		}
		isReloading = false;
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
/// will enable the collider
void Aweapon::drop(){
	Super::drop();
	resetFlags();
}

void Aweapon::dropToObjectPool(){
	drop();
	EntityManager *entityManager = worldLevel::entityManager();
	if(entityManager != nullptr){
		entityManager->add(this);
	}
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


	
		shootProtected(Start, End, teamEnum::none);//shoot from a start to an endpoint
	}
}

/// @brief shoot method for the bot, requires the bot to pickup the weapon(pickupBot method to save the pointer!)
/// will shoot the bot weapon 
/// @param target 
void Aweapon::shootBot(FVector target){
	if(botPointer != nullptr){
		FVector start = botPointer->GetActorLocation();
		FVector connect = (target - start).GetSafeNormal();
		start += connect * 100;

		teamEnum ownTeam = teamEnum::none;

		AHumanEntityScript *h = Cast<AHumanEntityScript>(botPointer);
		if(h != nullptr){
			ownTeam = h->getTeam();
		}

		shootProtected(start, target, ownTeam); // protected weapon shoot call
		releaseShoot(); //release for bot automatically
	}
}



/// @brief creates a raycast from start to end point and damages first object within the line
/// IS NOT DESIGNED TO BE CALLED FROM OUT SIDE! ONLY IN CLASS
/// @param Start pos
/// @param End pos target
void Aweapon::shootProtected(FVector Start, FVector End, teamEnum ownTeam){
	//FString::Printf(TEXT("subgraph size %d"), subgraph.size());
	
	if(canShoot()){ //check if can shoot

		//DONT FORGET THESE!! OTHERWISE NO FIRERATE IS APPLIED
		abzugHinten = true;
		resetCoolTime(cooldownTime);
		bulletsInMag--;

		// Perform the raycast
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // Ignore the character itself
		if(botPointer != nullptr){
			Params.AddIgnoredActor(botPointer);
		}
		Params.bTraceComplex = false; //new lower complexity

		

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
					teamEnum entityTeam = entity->getTeam();
					if(entityTeam != ownTeam || entityTeam == teamEnum::none){
						if(DEBUG_DRAW){
							DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
						}

						FVector hitpoint = HitResult.ImpactPoint;
						entity->takedamage(damageForAmmunitionType(), hitpoint); // must be changed later
					}else{
						//own team hit / any other
						if(DEBUG_DRAW){
							DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
						}
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
	}
}



void Aweapon::resetCoolTime(float time){
	//timeleft = time;

	timer.Begin(time);
}

/**
 * will return if the weapon is cooling at the moment
 */
bool Aweapon::isCooling(){
	//return (timer.timesUp() == false);

	return kickbackStarted || (timer.timesUp() == false);
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
	return !isCooling() && isActive() && (getMagSize() > bulletsInMag);
}

/**
 * reload the weapon with an amount of ammunition. negative numbers are ignored
 */
void Aweapon::reload(int amount){
	if(amount > 0){
		bulletsInMag += amount;
		resetCoolTime(reloadTime);
		reloadAnimation();

		isReloading = true;
	}
}

int Aweapon::getMagSize(){
	//return 30;
	if(Type == weaponEnum::assaultRifle){
		return 30;
	}
	if(Type == weaponEnum::pistol){
		return 12;
	}
	return 30;
}

int Aweapon::getBulletsInMag(){
	return bulletsInMag;
}

void Aweapon::showWeapon(bool show){
	Super::showItem(show);
	if(show){
		showAllPickedAttachments();
	}else{
		hideAllAttachments();
	}
}

weaponEnum Aweapon::readType(){
	return Type;
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
		//verschluss_anim_path = TEXT("/Game/Prefabs/weapons/rifle/verschlussAnim");
		//mag_anim_path = TEXT("/Game/Prefabs/weapons/rifle/magAnim");
		verschluss_anim_path = TEXT("/Game/Prefabs/weapons/rifle2/verschlussAnim");
		mag_anim_path = TEXT("/Game/Prefabs/weapons/rifle2/magAnim");
		gehauseAnimPath = TEXT("/Game/Prefabs/weapons/rifle2/gehauseAnim");

		magShootAnimpath = TEXT("/Game/Prefabs/weapons/rifle2/magAnimShoot");
	}

	setVerschlussPath(verschluss_anim_path);
	setMagAnimPath(mag_anim_path);


	//pre load the animations
	verschlussAnimationSquence = LoadObject<UAnimSequence>(nullptr, *verschluss_anim_path);
	magAnimationSequence = LoadObject<UAnimSequence>(nullptr, *mag_anim_path);
	gehauseAnimSequence = LoadObject<UAnimSequence>(nullptr, *gehauseAnimPath);
	magAnimationShootSequence = LoadObject<UAnimSequence>(nullptr, *magShootAnimpath);
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
			}else if(name.Contains("gehaeuse") || name.Contains("gehause")){
				gehauseSkeletonPointer = Component;
			}
			
			addIfIsAHandTarget(Component);
		}
	}
    
}

/// @brief plays the shoot animation if possible
void Aweapon::shootAnimation(){
	if(verschlussSkeletonPointer != nullptr){
		//old anim sequnce (new: custom animator)
		//playAnimation(verschlussAnimationSquence, verschlussSkeletonPointer, cooldownTime);

		if(false){
			//deprecated
			playAnimation(verschlussAnimationSquence, verschlussSkeletonPointer, cooldownTime);
			playAnimation(gehauseAnimSequence, gehauseSkeletonPointer, cooldownTime);
			playAnimation(magAnimationShootSequence, magSkeletonPointer, cooldownTime);

		}else{
			//new custom animation with target interpolator
			flagKickbackStart();
		}
	}
}

/// @brief plays the reload animation
void Aweapon::reloadAnimation(){
	if(magSkeletonPointer != nullptr){
		//playAnimation(magAnimPath, magSkeletonPointer, reloadTime);
		playAnimation(magAnimationSequence, magSkeletonPointer, reloadTime);
	}
}





// CAUTION: not in use anymore!

/// @brief plays an animation for an skeleton pointer and the anim sequence
/// @param AnimSequence 
/// @param skeleton 
/// @param time 
void Aweapon::playAnimation(
	UAnimSequence *AnimSequence, 
	USkeletalMeshComponent *skeleton,
	float time
){
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
	/*if(!canShoot()){
		return 0.0f;
	}
	return -0.1f;
	*/
	

	if(kickbackStarted && !recoilCopied){
		recoilCopied = true;
		return -0.1f;
	}

	//must be a negative value to properly flip up the camera!
	return 0.0f;




}






/// @brief finds all the attachments in blueprint for the weapon being DIRECT CHILD ACTOR in the weapon
void Aweapon::findAttachmentChildActors(){
	/*
	TArray<UChildActorComponent *> childs; //create a TArray of the targeted type
	GetComponents<UChildActorComponent>(childs); //collect all types with GetComponents<dt>(array) method
	if(childs.Num() > 0){
		for (int i = 0; i < childs.Num(); i++){
			if(childs[i] != nullptr){
				FString name = childs[i]->GetName();
				if(name.Contains("reddot")){
					DebugHelper::showScreenMessage("REDDOT FOUND", FColor::Red);
					reddotSightChildActor = childs[i];

					AActor *a = childs[i]->GetChildActor();
					if(a != nullptr){
						sightMap[weaponSightEnum::enum_reddot] = a;
					}
				}
				if(name.Contains("ironSight")){
					DebugHelper::showScreenMessage("IRON SIGHT FOUND", FColor::Red);
					ironSightChildActor = childs[i];

					AActor *a = childs[i]->GetChildActor();
					if(a != nullptr){
						sightMap[weaponSightEnum::enum_ironsight] = a;
					}
				}
			}
		}
	}


	//default value
	//applySight(weaponSightEnum::enum_ironsight);
	applySight(weaponAttachmentEnum::reddot);*/
}


void Aweapon::showAllPickedAttachments(){
	applySight(pickedSight);
	applyMuzzle(pickedMuzzle);
	applyGrip(pickedGrip);
}

void Aweapon::hideAllAttachments(){
	hideAllAttachments(sightMap);
	hideAllAttachments(gripMap);
	hideAllAttachments(muzzleMap);
}

/**
 * weapon setup helper api!
 */
/// @brief applys a sight if possible
/// @param sight sight value in to enable
void Aweapon::applySight(weaponAttachmentEnum sight){
	AActor *actorInMap = findFromMapAndEnable(sightMap, sight);
	if(actorInMap != nullptr){
		//sightActorPointer = a;
		AsightScript *tryCast = Cast<AsightScript>(actorInMap);

		if(tryCast){
			sightPointer = tryCast;
			findSightOffset();
		}else{
			resetSightOffset();
		}
	}

	pickedSight = sight;
}

/// @brief applys a muzzle attachment if possible
/// @param muzzle muzzle value in to enable
void Aweapon::applyMuzzle(weaponAttachmentEnum muzzle){
	AActor *actorInMap = findFromMapAndEnable(muzzleMap, muzzle);
	if(actorInMap != nullptr){
		
	}
	pickedMuzzle = muzzle;
}

void Aweapon::applyGrip(weaponAttachmentEnum type){
	AActor *actorInMap = findFromMapAndEnable(gripMap, type);
	if(actorInMap != nullptr){

	}
	pickedGrip = type;
}

AActor *Aweapon::findFromMapAndEnable(
	std::map<weaponAttachmentEnum, AActor*> &map,
	weaponAttachmentEnum type
){
	AActor *found = nullptr;
	for (auto &pair : map)
	{
		AActor *actorInMap = pair.second;
		if(actorInMap != nullptr){
			weaponAttachmentEnum typeCurrent = pair.first;
			bool show = (typeCurrent == type);
			AActorUtil::showActor(*actorInMap, show);

			if(show){
				found = actorInMap;
			}
		}
	}
	return found;
}

void Aweapon::hideAllAttachments(std::map<weaponAttachmentEnum, AActor*> &map){
	for (auto &pair : map)
	{
		AActor *actorInMap = pair.second;
		if(actorInMap != nullptr){
			bool show = false;
			AActorUtil::showActor(*actorInMap, show);
		}
	}
}






/**
 * spawning all attachments
 */
//new create sights all on start
void Aweapon::spawnAllAvailableAttachments(){

	loadAndSaveAttachment(weaponAttachmentEnum::reddot);
	loadAndSaveAttachment(weaponAttachmentEnum::iron_sight);

	loadAndSaveAttachment(weaponAttachmentEnum::grip_vertical);
	loadAndSaveAttachment(weaponAttachmentEnum::muzzle_flashSurpressor);
	loadAndSaveAttachment(weaponAttachmentEnum::muzzle_SoundSurpressor);
}

void Aweapon::loadAndSaveAttachment(weaponAttachmentEnum EattachmentType){

	assetManager *manager = assetManager::instance();
	EntityManager *entityManager = worldLevel::entityManager();
	if (manager != nullptr && entityManager != nullptr)
	{
		weaponEnum ownType = readType();

		//spawn uclass
		UClass *foundAttachment = manager->findBp(ownType, EattachmentType);
		if(foundAttachment != nullptr){
			FVector location;
			AActor *actor = entityManager->spawnAactor(GetWorld(), foundAttachment, location);
			if(actor != nullptr){
				
				attachNewItem(actor);
				if(weaponSetupHelper::isASightAttachment(EattachmentType)){
					sightMap[EattachmentType] = actor; //save pointer to map for enable disable
				}
				if(weaponSetupHelper::isAMuzzleAttachment(EattachmentType)){
					muzzleMap[EattachmentType] = actor;
				}
				if(weaponSetupHelper::isAGripAttachment(EattachmentType)){
					gripMap[EattachmentType] = actor;
				}
				
			}
		}
	}
}







/// @brief returns the ammunition type for this weapon
/// @return 
ammunitionEnum Aweapon::getAmmunitionType(){
	weaponEnum type = readType();
	switch(type){
	case weaponEnum::assaultRifle:
		return ammunitionEnum::assaultrifle556;

	case weaponEnum::pistol:
		return ammunitionEnum::pistol9;
	}

	return ammunitionEnum::assaultrifle556;
}






/**
 * 
 * Bone controller attachment / socket locations!
 * 
 */


/// @brief right hand may be named "righthand"
/// 	   right hand subparts may be named "righthand_thumb"!! MUST FOLLOW THIS NAMING!
/// @param Component 
void Aweapon::addIfIsAHandTarget(USkeletalMeshComponent *Component){
	if(Component){
		FString name = Component->GetName();
		if(name.Contains("righthand")){
			HandBoneIndexEnum enumCheckup = socketNameToEnum(name);
			if(enumCheckup == HandBoneIndexEnum::none){ //
				rightHandTargetSkelletonPointer = Component;
			}else{
				addToFingerTargetMap(
					right_fingerTargetsMap,
					enumCheckup,
					Component
				);
			}
		}
		else if(name.Contains("lefthand")){
			HandBoneIndexEnum enumCheckup = socketNameToEnum(name);
			if(enumCheckup == HandBoneIndexEnum::none){ //
				leftHandTargetSkelletonPointer = Component;
			}else{
				addToFingerTargetMap(
					left_fingerTargetsMap,
					enumCheckup,
					Component
				);
			}
		}
	}
}

/// @brief will try to find the finger name from the string, returns "none" if no matches
/// @param name name to check
/// @return none if none found
HandBoneIndexEnum Aweapon::socketNameToEnum(FString &name){
	if(name.Contains("thumb"))
		return HandBoneIndexEnum::thumb;
	if(name.Contains("finger1"))
		return HandBoneIndexEnum::finger1;
	if(name.Contains("finger2"))
		return HandBoneIndexEnum::finger2;
	if(name.Contains("finger3"))
		return HandBoneIndexEnum::finger3;
	if(name.Contains("finger4"))
		return HandBoneIndexEnum::finger4;

	return HandBoneIndexEnum::none;
}

void Aweapon::addToFingerTargetMap(
	std::map<HandBoneIndexEnum, USkeletalMeshComponent *> &map,
	HandBoneIndexEnum fingerIndex,
	USkeletalMeshComponent *component
){
	if(component != nullptr){
		map[fingerIndex] = component;
	}
}







FVector Aweapon::leftHandLocation(){

	if(isReloading){
		if (magSkeletonPointer != nullptr)
		{
			//DebugHelper::showScreenMessage("reload location", FColor::Purple);
			FVector componentWorldLocation = magSkeletonPointer->GetComponentLocation();


			/**
			 * get the bon location of "Mag" inside the skelletal component
			 * pointer to get the real world location of this "bone"
			 * and not the component itself
			 */
			FVector BoneLocation = magSkeletonPointer->GetBoneLocation(TEXT("Mag"));
			return BoneLocation;
			//return componentWorldLocation;
		}
	}
	
	
	if(leftHandTargetSkelletonPointer != nullptr){
		// Get the world location of the Skeletal Mesh Component
    	FVector componentWorldLocation = leftHandTargetSkelletonPointer->GetComponentLocation();
		return componentWorldLocation;
	}

	return GetActorLocation();
}

FVector Aweapon::rightHandLocation(){
	if(rightHandTargetSkelletonPointer != nullptr){
		// Get the world location of the Skeletal Mesh Component
    	FVector componentWorldLocation = rightHandTargetSkelletonPointer->GetComponentLocation();
		return componentWorldLocation;
	}

	return GetActorLocation();
}


void Aweapon::loadFingerTargets(HandTargetContainer &container){
	Super::loadFingerTargets(container);

	
	MMatrix handRotator;
	HandBoneIndexEnum handType = container.readHandtype();
	if(handType == HandBoneIndexEnum::leftHand){
		handRotator.rollRadAdd(MMatrix::degToRadian(90.0f));
	}
	if(handType == HandBoneIndexEnum::rightHand){
		handRotator.rollRadAdd(MMatrix::degToRadian(-90.0f));
	}
	container.setOrientation(handRotator);

}

FVector Aweapon::leftHandFingerLocation(HandBoneIndexEnum type){
	//DebugHelper::showScreenMessage("load fingers left", FColor::Red);
	/**
	 *  achtung: es braucht einen seperaten container der speichert wo die finger positionen sind
		bzw weitere skelletal mesh components
		weil finger auch an mehreren stellen anbringbar sind

		es braucht sowas wie ein „objekt finger position pair"!
	 * 
	 */
	if (left_fingerTargetsMap.find(type) != left_fingerTargetsMap.end()) {
		// Key exists
	}
	return leftHandLocation();
}

FVector Aweapon::rightHandFingerLocation(HandBoneIndexEnum type){
	//DebugHelper::showScreenMessage("load fingers right", FColor::Purple);
	if (right_fingerTargetsMap.find(type) != right_fingerTargetsMap.end()) {
		// Key exists
		USkeletalMeshComponent *comp = right_fingerTargetsMap[type];
		if(comp != nullptr){
			//DebugHelper::showScreenMessage("load fingers right", FColor::Purple);
			FVector pos = comp->GetComponentLocation();
			//DebugHelper::showScreenMessage(pos);
			return pos;
		}
	}
	return rightHandLocation();
}



int Aweapon::damageForAmmunitionType(){
	ammunitionEnum typethis = getAmmunitionType();
	if(typethis == ammunitionEnum::assaultrifle556){
		return 18;
	}
	if(typethis == ammunitionEnum::pistol9){
		return 15;
	}
	if(typethis == ammunitionEnum::heavy762){
		return 21;
	}
	return 10;
}






/**
 * 
 * 
 * --- new attachment section ---
 * 
 */

///@brief will attach new item IF the gehauseSkelleton pointer exists!
void Aweapon::attachNewItem(AActor* actor){
	if(actor == nullptr){
		return;
	}
	if(!actorAlreadyAttached(actor)){
		
		attachedActors.push_back(actor);

		// IST DAS SELBE WIE AUS EINEM BLUEPRINT MANUELL HINZUFÜGEN
		if (gehauseSkeletonPointer)
		{
			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
			actor->AttachToComponent(gehauseSkeletonPointer, AttachRules);
		}
	}
}

bool Aweapon::actorAlreadyAttached(AActor *actor){
	for (int i = 0; i < attachedActors.size(); i++){
		if(actor == attachedActors[i]){
			return true;
		}
	}
	return false;
}








/**
 * 
 * 
 * --- new testing keyframe anim on actor ---
 * 
 */
void Aweapon::flagKickbackStart(){
	verschlussKickBackStarted = true;
	kickbackStarted = true;
	recoilCopied = false;
}

void Aweapon::setupKickBackAnimation(){
	int kickBackDistance = 3; //3cm

	actorKickBackAnim = KeyFrameAnimation(false); // instant flip animation, dont loop at end to start
	actorKickBackAnim.useHermiteSplineInterpolation(false); //linear default

	actorKickBackAnim.addFrame(
		FVector(0, 0, 0),
		0.0f, // time to prev frame
		false);
	actorKickBackAnim.addFrame(
		FVector(-kickBackDistance, 0, 0), //x forward
		cooldownTime * 0.1f, //time to prev frame
		false
	);
	actorKickBackAnim.addFrame(
		FVector(0, 0, 0),
		cooldownTime * 0.9f, //time to prev frame
		false
	);
}

bool Aweapon::kickbackIsRunning(){
	return kickbackStarted;
}

void Aweapon::TickKickback(float DeltaTime){
	if(kickbackIsRunning()){
		FVector newOffset = actorKickBackAnim.interpolate(DeltaTime);

		//bone controller motion queue will handle the anim state
		Super::updateAnimationOffset(newOffset);

		if(actorKickBackAnim.reachedLastFrameOfAnimation()){
			kickbackStarted = false;
		}
	}else{
		//nicht nötig (?)
		FVector none;
		Super::updateAnimationOffset(none);
	}
}




/**
 * 
 * -- verschluss animation --
 * 
 */
void Aweapon::setupVerschlussAnimation(){
	verschlussKickBackAnimation = KeyFrameAnimation(false);
	verschlussKickBackAnimation.useHermiteSplineInterpolation(false); //linear default

	FVector currentRelativeLocation;
	if (verschlussSkeletonPointer != nullptr)
	{
		currentRelativeLocation = verschlussSkeletonPointer->GetRelativeLocation();
	}
	

	int kickBackDistance = 3;
	verschlussKickBackAnimation.addFrame(
		currentRelativeLocation + FVector(0, 0, 0),
		0.0f, // time to prev frame
		false);
	verschlussKickBackAnimation.addFrame(
		currentRelativeLocation + FVector(-kickBackDistance, 0, 0), //x forward
		cooldownTime * 0.1f, //time to prev frame
		false
	);
	verschlussKickBackAnimation.addFrame(
		currentRelativeLocation + FVector(0, 0, 0),
		cooldownTime * 0.8f, //time to prev frame
		false
	);
}


void Aweapon::TickVerschlussKickBack(float DeltaTime){
	if(verschlussKickBackStarted && verschlussSkeletonPointer != nullptr){

		FVector interpolatedLocal = verschlussKickBackAnimation.interpolate(DeltaTime);
		verschlussSkeletonPointer->SetRelativeLocation(interpolatedLocal);

		if(verschlussKickBackAnimation.reachedLastFrameOfAnimation()){
			verschlussKickBackStarted = false;
		}
	}
}




/**
 * sight offset - is tested!
 */
void Aweapon::findSightOffset(){
	
	if(sightPointer){
	//if(sightActorPointer){
		FVector A = sightPointer->GetActorLocation();
		//FVector A = sightActorPointer->GetActorLocation(); //is world(?)
		FVector B = GetActorLocation();

		float verticalDistance = B.Z - A.Z; //AB = B - A

		verticalSightOffset = FVector(0.0f, 0.0f, verticalDistance);

		DebugHelper::logMessage("debugsight offset: ", verticalDistance);
	}
}

void Aweapon::resetSightOffset(){
	verticalSightOffset = FVector(0.0f, 0.0f, 0.0f);
}
