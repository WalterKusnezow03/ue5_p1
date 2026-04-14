// Fill out your copyright notice in the Description page of Project Settings.

#include "weapon.h"
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"
#include "p2/entityManager/AlertManager.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/player/playerScript.h"
#include "sightScript.h"
#include "ammunitionEnum.h"
#include <map>
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/team/teamEnum.h"
#include "p2/entities/HumanEntityScript.h"
#include "GameCore/util/AActorUtil.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/EntityManager.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"

#include "CoreMath/util/Raycaster.h"


#include "p2/weapon/enumUtil/WeaponEnumAssetPackProxy.h"


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

	
	muzzleAttachmentSkelletonPointer = nullptr;
	gripAttachmentSkelletonPointer = nullptr;
	gehauseSkeletonPointer = nullptr;

	reddotSightChildActor = nullptr;
	ironSightChildActor = nullptr;

	// Ensure the World context is valid
	isAiming = false;
	singleFireModeOn = false;

	offset = FVector(-100, 100.0f, 0);


	pickedMuzzle = weaponAttachmentEnum::muzzle_flashSurpressor;
}

void Aweapon::resetFlags(){
	kickbackStarted = false;
	//verschlussKickBackStarted = false;
	verschlussState = EVerschlussState::ENone;
	recoilCopied = false;
	abzugHinten = false; 
	isReloading = false;
}

weaponEnum Aweapon::weaponType() const {
	return Type;
}




/**
 * calculates time to wait for a int of rounds per minute
 */
float Aweapon::calculateRpm(int rpm){
	float rps = rpm / 60.0f;
	return 1.0f / rps; //1s / rps = wait time in seconds
}

// Called when the game starts or when spawned
void Aweapon::BeginPlay()
{
	Super::BeginPlay();

	setupAllComponentsAndAnimationsOnBeginPlay();


	enableCollider(true);
	isVisible = true; //inital setting of visibilty, do not remove!


	//only on begin play!
	spawnAllAvailableAttachments();

	setupKickBackAnimation();
	setupVerschlussAnimations();


	setupSwayAnimation();


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
	TickSway(DeltaTime);
	NiagaraTickAll(DeltaTime);
}


// --- IK CARRY INTERFACE OVERRIDE
void Aweapon::UpdateActorTransform(FVector &location, FRotator &rotation){
	FVector accumulated = location + sightOffsetNoRotation();
	Super::UpdateActorTransform(accumulated, rotation);
}

//depreacted

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
	//new, on finish:
	if(timer.TickWithTimesUpReset(time)){
		if(!singleFireMode()){
			abzugHinten = false; 
		}

		VerschlussToFrontAfterReloadEmpty();

		isReloading = false;
	}

}

void Aweapon::VerschlussToFrontAfterReloadEmpty(){
	if(isEmptyReload && isReloading){
		verschlussState = EVerschlussState::EEmptyReloadRevert;
		DebugHelper::logMessage("Aweapon::VerschlussToFrontAfterReloadEmpty");
	}
}

/// @brief Unbind from player or bot
/// will unbind the weapon from the camera or bot Pointer passed when picking up the weapon
/// will enable the collider
void Aweapon::drop(){
	Super::drop();
	resetFlags();
    RegisterToMiniMap(); //register on drop
}

void Aweapon::dropToObjectPool(){
	drop();
	EntityManager *entityManager = AworldLevel::entityManager();
	if(entityManager != nullptr){
		entityManager->add(this);
	}
	UnRegisterFromMiniMap();
}

/// @brief releases the shot (mouse up)
void Aweapon::releaseShoot(){
	abzugHinten = false;
}

/// @brief shoot method for player! make sure camera is attached!
void Aweapon::shoot(){
	if(cameraPointer != nullptr && canShoot()){
		//component world look dir
		FVector ForwardVector = cameraPointer->GetForwardVector();
    	
		//Component World Location
    	FVector CameraLocation = cameraPointer->GetComponentLocation();
    	
		// Define the start and end vectors for the raycast
		FVector Start = CameraLocation + ForwardVector * 100; // todo: owner also by interface! to exclude from query

		float sizeRay = 50000.0f;
		shootProtected(Start, ForwardVector, sizeRay, teamEnum::none); // shoot from a start to an endpoint
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

		float sizeRay = 50000.0f;
		shootProtected(start, connect, sizeRay, ownTeam); // protected weapon shoot call
		releaseShoot(); //release for bot automatically
	}
}



bool Aweapon::shootProtected(FVector Start, FVector dir, float sizeRay, teamEnum ownTeam){
	return shootProtected(Start, dir, sizeRay, ownTeam, true);
}

/// @brief creates a raycast from start to end point and damages first object within the line
/// IS NOT DESIGNED TO BE CALLED FROM OUT SIDE! ONLY IN CLASS
/// @param Start pos
/// @param End pos target
bool Aweapon::shootProtected(FVector Start, FVector dir, float sizeRay, teamEnum ownTeam, bool damageOnHit){
	//FString::Printf(TEXT("subgraph size %d"), subgraph.size());
	
	if(canShoot()){ //check if can shoot

		//DONT FORGET THESE!! OTHERWISE NO FIRERATE IS APPLIED
		abzugHinten = true;
		resetCoolTime(cooldownTime());
		bulletsInMag--;

		bool traceComplex = false;
		FHitResult HitResult;
		FVector outputHitPointIgnored;
		Raycaster caster;
		bool bHit = caster.performRaycast(
			GetWorld(), 
			Start, 
			dir,
			GetFCollisionQueryParams(), //AcarriedItem::GetFCollisionQueryParams
			sizeRay,
			outputHitPointIgnored,
			HitResult,
			traceComplex
		);


		// If the raycast hit something, save hitresult and return positive
		if (bHit)
		{
			latestHit = HitResult;
			AActor *actor = HitResult.GetActor();
			//damage if only flagged as damage.
			if(damageOnHit){
				damageIfPossible(ownTeam, actor, HitResult, Start, dir, sizeRay);
			}
			
			
		}


		//play animation
		shootAnimation();

		if(!isSoundSurpressed()){
			float distance = 50000; //50 * 100cm = 50m
			if(AAlertManager *instance = AAlertManager::Instance(GetWorld())){
				instance->alertInArea(GetActorLocation(), distance);
			}
		}
		NiagaraTriggerMuzzleFlash();
		NiagaraTriggerHuelseEject();
		


		return bHit;
	}
	return false;
}



void Aweapon::damageIfPossible(
	teamEnum ownTeam,
	AActor *actor,
	FHitResult &hitresult,
	FVector &start,
	FVector &dir,
	float sizeRay
){
	FVector end = start + dir * sizeRay;
	damageIfPossible(ownTeam, actor, hitresult, start, end);
}

void Aweapon::damageIfPossible(
	teamEnum ownTeam,
	AActor *actor,
	FHitResult &hitresult,
	FVector &start,
	FVector &end
){
	if(actor != nullptr){
		IDamageinterface* entity = Cast<IDamageinterface>(actor);
		damageIfPossible(ownTeam, entity, hitresult, start, end);
	}
}

void Aweapon::damageIfPossible(
	teamEnum ownTeam,
	IDamageinterface *entity, 
	FHitResult &HitResult, 
	FVector &start, 
	FVector &end
){
	if(entity){
		//damage entity if some other team
		teamEnum entityTeam = entity->getTeam();
		if(entityTeam != ownTeam || entityTeam == teamEnum::none){
			if(DEBUG_DRAW){
				DrawDebugLine(GetWorld(), start, end, FColor::Purple, false, 1.0f, 0, 1.0f);
			}

			FCustomHitResult hitResultPackage = MakeHitResult(HitResult, start);
			entity->takedamage(hitResultPackage);
				
		}else{
			//own team hit / any other
			if(DEBUG_DRAW){
				DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 1.0f, 0, 1.0f);
			}
		}
	}
}


FCustomHitResult Aweapon::MakeHitResult(
	FHitResult &HitResult, 
	FVector &start
){
	FVector hitpoint = HitResult.ImpactPoint;
	float damage = WeaponEnumAssetPackProxy::damageFor(Type, start, hitpoint);

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FCustomHitResult hitResultPackage;
	hitResultPackage.SetupHitResult(
		HitResult,
		damage, 
		isSoundSurpressed(), 
		start,
		DeltaTime
	);
	return hitResultPackage;
}








FVector Aweapon::LatestHitLocation(){
	return latestHit.ImpactPoint;
}
FVector Aweapon::LatestHitNormal(){
	return latestHit.Normal;
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

bool Aweapon::isSoundSurpressed(){
	return pickedMuzzle == weaponAttachmentEnum::muzzle_SoundSurpressor;
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

	return kickbackIsRunning() || (timer.timesUp() == false);
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
	FString reason;
	if(!canReload(reason)){
		DebugHelper::logMessage(reason);
	}
	return true;

	//return !isCooling() && isActive() && (getMagSize() > bulletsInMag);
}

bool Aweapon::canReload(FString &reason){
	if(isCooling()){
		reason = TEXT("Aweapon::reload cant reload, cooling ") + WeaponTypeToString();
		return false;
	}
	if(getMagSize() <= bulletsInMag){
		reason = TEXT("Aweapon::reload cant reload, enough bullets ") + WeaponTypeToString();
		return false;
	}
	reason = TEXT("- allowed -");
	DebugHelper::logMessage("Aweapon::reload can reload ", WeaponTypeToString());
	return true;

}





/**
 * reload the weapon with an amount of ammunition. negative numbers are ignored
 */

void Aweapon::reload(int amount){
	if(amount > 0){
		isEmptyReload = (bulletsInMag <= 0);
		if(isEmptyReload){
			verschlussState = EVerschlussState::ENone;
		}
		

		bulletsInMag += amount;
		resetCoolTime(reloadTime());
		reloadAnimation();

		isReloading = true;

		//add payload to timer: reset hand targets

		Payload generated = handAndFingerPositionManager.UpdateTemporaryTargetWithPaylaod(
			EArmType::ELeft, 
			magSkeletonPointer,
			"mag" //bone name
		);
		timer.AddPayload(generated);



		//DEBUG
		FString message = FString::Printf(
			TEXT("Aweapon::reload! type: %s: amount: %d"),
			*WeaponTypeToString(),
			amount
		);

		DebugHelper::logMessage(message);
	}
}

FString Aweapon::WeaponTypeToString(){
	return WeaponEnumAssetPackProxy::toString(weaponType());
}


// --------- weapon properties usuage -----------


int Aweapon::getMagSize(){
	return WeaponEnumAssetPackProxy::getMagSize(weaponType());
}

int Aweapon::getBulletsInMag(){
	return bulletsInMag;
}


/// @brief will return a recoil value to apply if recoil not copied yet
/// @return value (negative) for camera roatation, or 0 if cant shoot at the moment
float Aweapon::recoilValue(){

	//copy recoil whether still kickback or doesnt!
	if(!recoilCopied){
		//must be a negative value to properly flip up the camera!
		recoilCopied = true;
		
		return WeaponEnumAssetPackProxy::recoilValue(weaponType());
		
		//const WeaponProperties &ref = WeaponPropertiesMap::findProperty(weaponType());
		//return ref.recoilValue();
	}
	return 0.0f;

}

float Aweapon::cooldownTime(){
	//const WeaponProperties &ref = WeaponPropertiesMap::findProperty(weaponType());
	//return ref.CooldownTimeBasedOnRpm();

	return WeaponEnumAssetPackProxy::CooldownTimeBasedOnRpm(weaponType());
}

float Aweapon::reloadTime(){
	//const WeaponProperties &ref = WeaponPropertiesMap::findProperty(weaponType());
	//return ref.reloadTime();

	return WeaponEnumAssetPackProxy::reloadTime(weaponType());
}

/// @brief used for player inventory reloading
/// @return type
ammunitionEnum Aweapon::getAmmunitionType(){
	//const WeaponProperties &ref = WeaponPropertiesMap::findProperty(weaponType());
	//return ref.getAmmunitionType();

	return WeaponEnumAssetPackProxy::getAmmunitionType(weaponType());
}

/// @brief will say if single fire is on or default always on
/// @return true false
bool Aweapon::singleFireMode(){
	//given single fire mode
	//const WeaponProperties &ref = WeaponPropertiesMap::findProperty(weaponType());
	//if(ref.isSingleFireOnly()){
	//	return true;
	//}

	if(WeaponEnumAssetPackProxy::isSingleFireOnly(weaponType())){
		return true;
	}

	//custom single fire mode
	return singleFireModeOn;
}












void Aweapon::showWeapon(bool show){
	Super::showItem(show);
	if(show){
		showAllPickedAttachments();
	}else{
		hideAllAttachments();
	}
}



#include "p2/weapon/animationEnum/EweaponAnimation.h"
void Aweapon::LoadAnimationsFromAssetManager(){
	if(assetManager *manager = assetManager::instance()){

		//pre load the animations
		verschlussAnimationSquence = manager->Find<weaponEnum, EweaponAnimation, UAnimSequence>(
			Type, //type weapon
			EweaponAnimation::verschlussAnimationShoot
		);

		magAnimationSequence = manager->Find<weaponEnum, EweaponAnimation, UAnimSequence>(
			Type, //type weapon
			EweaponAnimation::magAnimationReload
		);
		gehauseAnimSequence = manager->Find<weaponEnum, EweaponAnimation, UAnimSequence>(
			Type, //type weapon
			EweaponAnimation::gehauseAnimationShoot
		);
		magAnimationShootSequence = manager->Find<weaponEnum, EweaponAnimation, UAnimSequence>(
			Type, //type weapon
			EweaponAnimation::magAnimationShoot
		);

		DebugHelper::logMessage("Aweapon::LoadAnimationsFromAssetManager");
	}
}






/// @brief setups all components for the animations
void Aweapon::setupAllComponentsAndAnimationsOnBeginPlay()
{
	LoadAnimationsFromAssetManager();
	findAllSkeletalAndSceneComponents();
}

void Aweapon::findAllSkeletalAndSceneComponents(){
	findAllSkeletalComponents();
	findAllNiagaraComponents();
}

void Aweapon::findAllSkeletalComponents(){
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
			}else if(name.Contains("muzzle")){
				muzzleAttachmentSkelletonPointer = Component;
			}else if(name.Contains("grip")){
				gripAttachmentSkelletonPointer = Component;
			}else if(name.Contains("sight")){
				sightAttachmentSkeletonPointer = Component;
			}
		}
	}
}

void Aweapon::findAllNiagaraComponents(){
	// --- MUZZLE FLASH ---
	FindNiagaraMuzzleFlash();
	FindNiagaraHuelseEject();
}

void Aweapon::FindNiagaraMuzzleFlash(){
	// --- MUZZLE FLASH ---
	TTryAssignByNameContains<UNiagaraComponentCustom>(
		"niagaraMuzzleFlash",
		niagaraMuzzleFlash
	);
	if(niagaraMuzzleFlash){
		niagaraMuzzleFlash->timeOfEffect = cooldownTime();
		niagaraMuzzleFlash->StopEffectImmediate();
	}
}

void Aweapon::FindNiagaraHuelseEject(){
	// --- HUELSE EJECT ---
	TTryAssignByNameContains<UNiagaraComponentCustom>(
		"niagaraHuelse",
		niagaraHuelseEject
	);
	if(niagaraHuelseEject){
		niagaraHuelseEject->timeOfEffect = 5.0f; 
		niagaraHuelseEject->StopEffectImmediate();
	}
}




void Aweapon::NiagaraTickAll(float deltatime){
	if(niagaraMuzzleFlash){
		niagaraMuzzleFlash->TickExternal(deltatime);
	}
	if(niagaraHuelseEject){
		niagaraHuelseEject->TickExternal(deltatime);
	}
}




void Aweapon::NiagaraTriggerMuzzleFlash(){
	if(niagaraMuzzleFlash){
		niagaraMuzzleFlash->ResetAndRestart();
	}
}

void Aweapon::NiagaraTriggerHuelseEject(){
	if(niagaraHuelseEject){
		niagaraHuelseEject->ResetAndRestart();
	}
}












/// @brief plays the shoot animation if possible
void Aweapon::shootAnimation(){
	
	if(false && verschlussSkeletonPointer != nullptr){
		//old anim sequnce (new: custom animator)
		//playAnimation(verschlussAnimationSquence, verschlussSkeletonPointer, cooldownTime);

		if(false){
			//deprecated
			playAnimation(verschlussAnimationSquence, verschlussSkeletonPointer, cooldownTime());
			playAnimation(gehauseAnimSequence, gehauseSkeletonPointer, cooldownTime());
			playAnimation(magAnimationShootSequence, magSkeletonPointer, cooldownTime());

		}
	}


	//new custom animation with target interpolator
	flagKickbackStart();

}

/// @brief plays the reload animation
void Aweapon::reloadAnimation(){
	if(magSkeletonPointer != nullptr){
		//play animation on skeletal mesh component with the UAnimation Sequence.
		playAnimation(magAnimationSequence, magSkeletonPointer, reloadTime());
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













/// ---- ATTACHMENT SECTION -----

/// @brief spawns all available attachments and disables the not picked ones
void Aweapon::spawnAllAvailableAttachments(){

	loadAndSaveAttachment(weaponAttachmentEnum::reddot);
	loadAndSaveAttachment(weaponAttachmentEnum::iron_sight);

	loadAndSaveAttachment(weaponAttachmentEnum::grip_vertical);
	loadAndSaveAttachment(weaponAttachmentEnum::muzzle_flashSurpressor);
	loadAndSaveAttachment(weaponAttachmentEnum::muzzle_SoundSurpressor);
}

/// @brief loads an attachment by type, spawns it and attaches it to the 
/// proper skelletal part as attachment
/// @param EattachmentType attachment to spawn
void Aweapon::loadAndSaveAttachment(weaponAttachmentEnum EattachmentType){

	assetManager *assetManagerInstance = assetManager::instance();
	EntityManager *entityManager = AworldLevel::entityManager();
	if (assetManagerInstance != nullptr && entityManager != nullptr)
	{
		weaponEnum ownType = weaponType();

		//spawn uclass
		UClass *foundAttachment = 
			assetManagerInstance->Find<weaponEnum, weaponAttachmentEnum, UClass>(ownType, EattachmentType);
		// assetManagerInstance->findBp(ownType, EattachmentType);
		if(foundAttachment != nullptr){
			FVector location;
			AActor *actor = entityManager->spawnAactor(GetWorld(), foundAttachment, location);
			if(actor != nullptr){
				
				attachNewItem(actor, EattachmentType);
				if(WeaponEnumAssetPackProxy::isASightAttachment(EattachmentType)){
					sightMap[EattachmentType] = actor; //save pointer to map for enable disable
				}
				if(WeaponEnumAssetPackProxy::isAMuzzleAttachment(EattachmentType)){
					muzzleMap[EattachmentType] = actor;
				}
				if(WeaponEnumAssetPackProxy::isAGripAttachment(EattachmentType)){
					gripMap[EattachmentType] = actor;
				}
				
			}
		}
	}
}




/// @brief shows all selected attachments and hides the other ones
void Aweapon::showAllPickedAttachments(){
	applySight(pickedSight);
	applyMuzzle(pickedMuzzle);
	applyGrip(pickedGrip);
}

/// @brief hides all attached actors because visibility setting is not propagated!
void Aweapon::hideAllAttachments(){
	hideAllAttachments(sightMap);
	hideAllAttachments(gripMap);
	hideAllAttachments(muzzleMap);
}

/**
 * -------- weapon setup helper api! ---------
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
		pickedMuzzle = muzzle;
	}
	
}

void Aweapon::applyGrip(weaponAttachmentEnum type){
	AActor *actorInMap = findFromMapAndEnable(gripMap, type);
	if(actorInMap != nullptr){
		pickedGrip = type;
	}
	
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




// ---- attach to actor sockets start ----

///@brief will attach new item IF the gehauseSkelleton pointer exists!
void Aweapon::attachNewItem(AActor* actor){
	if(actor == nullptr){
		return;
	}
	if(!actorAlreadyAttached(actor)){
		
		attachedActors.push_back(actor);

		AActorUtil::enableColliderOnActor(*actor, false);

		// IST DAS SELBE WIE AUS EINEM BLUEPRINT MANUELL HINZUFÜGEN
		if (gehauseSkeletonPointer)
		{
			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
			actor->AttachToComponent(gehauseSkeletonPointer, AttachRules);
		}
	}
}




///@brief will attach new item IF the skelleton pointer responsible for the attachment type exists!
void Aweapon::attachNewItem(AActor* actor, weaponAttachmentEnum type){
	if(actor == nullptr){
		return;
	}
	if(!actorAlreadyAttached(actor)){
		attachedActors.push_back(actor);

		AActorUtil::enableColliderOnActor(*actor, false);

		// IST DAS SELBE WIE AUS EINEM BLUEPRINT MANUELL HINZUFÜGEN
		USkeletalMeshComponent *ptr = attachmentSkeletalComponentBy(type);
		if(ptr){
			FAttachmentTransformRules AttachRules(EAttachmentRule::KeepRelative, true);
			actor->AttachToComponent(ptr, AttachRules);
		}
		
	}
}

///@brief returns the skeletal mesh component pointer for an given attachment 
USkeletalMeshComponent* Aweapon::attachmentSkeletalComponentBy(
	weaponAttachmentEnum EattachmentType
){
	if(WeaponEnumAssetPackProxy::isASightAttachment(EattachmentType)){
		//return explicit attachment pointer if valid.
		if(sightAttachmentSkeletonPointer){
			return sightAttachmentSkeletonPointer;
		}
		//default attachment to 0,0,0 of weapon.
		return gehauseSkeletonPointer;
	}
	if(WeaponEnumAssetPackProxy::isAMuzzleAttachment(EattachmentType)){
		return muzzleAttachmentSkelletonPointer;
	}
	if(WeaponEnumAssetPackProxy::isAGripAttachment(EattachmentType)){
		return gripAttachmentSkelletonPointer;
	}
	return gehauseSkeletonPointer;
}

bool Aweapon::actorAlreadyAttached(AActor *actor){
	for (int i = 0; i < attachedActors.size(); i++){
		if(actor == attachedActors[i]){
			return true;
		}
	}
	return false;
}

// ---- attach to actor sockets end ----









/**
 * 
 * 
 * --- new testing keyframe anim on actor ---
 * 
 */
void Aweapon::flagKickbackStart(){
	kickbackStarted = true;
	recoilCopied = false;

	flagKickbackStartVerschluss();
}

void Aweapon::flagKickbackStartVerschluss(){
	int bullets = getBulletsInMag();
	DebugHelper::logMessage("Aweapon::flagKickbackStartVerschluss bullets: ", bullets);

	if(bullets > 0){
		verschlussState = EVerschlussState::EDefaultKickback;
	}else{
		verschlussState = EVerschlussState::EEmptyMagKickback;
	}
}






void Aweapon::setupKickBackAnimation(){
	ClearKickbackAnimation();
	int kickBackDistance = 10; // 3cm
	actorKickBackAnim.addFrame(
		FVector(0, 0, 0),
		0.0f, // time to prev frame
		false);
	actorKickBackAnim.addFrame(
		FVector(-kickBackDistance, 0, 0), //x forward
		cooldownTime() * 0.1f, //time to prev frame
		false
	);
	actorKickBackAnim.addFrame(
		FVector(0, 0, 0),
		cooldownTime() * 0.9f, //time to prev frame
		false
	);
}

void Aweapon::ClearKickbackAnimation(){

	actorKickBackAnim = KeyFrameAnimation(false); // instant flip animation, dont loop at end to start
	actorKickBackAnim.useHermiteSplineInterpolation(false); //linear default
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

void Aweapon::setupVerschlussAnimations(){
	FVector currentRelativeLocation;
	if (verschlussSkeletonPointer != nullptr){
		currentRelativeLocation = verschlussSkeletonPointer->GetRelativeLocation();
	}
	setupVerschlussAnimation(currentRelativeLocation);
	setupVerschlussAnimationLastShot(currentRelativeLocation);
	setupVerschlussKickBackAnimationLastShotEmptyReload(currentRelativeLocation);
}

void Aweapon::setupVerschlussAnimation(const FVector &currentRelativeLocation){
	verschlussKickBackAnimation = KeyFrameAnimation(false);
	verschlussKickBackAnimation.useHermiteSplineInterpolation(false); //linear default
	
	verschlussKickBackAnimation.addFrame(
		currentRelativeLocation + FVector(0, 0, 0),
		0.0f, // time to prev frame
		false);
	verschlussKickBackAnimation.addFrame(
		currentRelativeLocation + FVector(-VerschlussKickbackDistance, 0, 0), //x forward
		cooldownTime() * 0.1f, //time to prev frame
		false
	);
	verschlussKickBackAnimation.addFrame(
		currentRelativeLocation + FVector(0, 0, 0),
		cooldownTime() * 0.8f, //time to prev frame
		false
	);
}

void Aweapon::setupVerschlussAnimationLastShot(const FVector &currentRelativeLocation){
	verschlussKickBackAnimationLastShot = KeyFrameAnimation(false);
	verschlussKickBackAnimationLastShot.useHermiteSplineInterpolation(false); //linear default

	verschlussKickBackAnimationLastShot.addFrame(
		currentRelativeLocation + FVector(0, 0, 0),
		0.0f, // time to prev frame
		false);
	verschlussKickBackAnimationLastShot.addFrame(
		currentRelativeLocation + FVector(-VerschlussKickbackDistance, 0, 0), //x forward
		cooldownTime() * 0.1f, //time to prev frame
		false
	);
}

void Aweapon::setupVerschlussKickBackAnimationLastShotEmptyReload(const FVector &currentRelativeLocation){
	verschlussKickBackAnimationLastShotEmptyReload = KeyFrameAnimation(false);
	verschlussKickBackAnimationLastShotEmptyReload.useHermiteSplineInterpolation(false); //linear default
	
	verschlussKickBackAnimationLastShotEmptyReload.addFrame(
		currentRelativeLocation + FVector(-VerschlussKickbackDistance, 0, 0), //x forward
		0.0f, //time to prev frame
		false
	);

	verschlussKickBackAnimationLastShotEmptyReload.addFrame(
		currentRelativeLocation + FVector(0, 0, 0),
		cooldownTime() * 1.0f, // time to prev frame
		false
	);
}








void Aweapon::TickVerschlussKickBack(float DeltaTime){
	if(!verschlussSkeletonPointer){
		return;
	}

	//deprecated!
	/*
	if(verschlussKickBackStarted && verschlussSkeletonPointer != nullptr){

		FVector interpolatedLocal = verschlussKickBackAnimation.interpolate(DeltaTime);
		verschlussSkeletonPointer->SetRelativeLocation(interpolatedLocal);

		if(verschlussKickBackAnimation.reachedLastFrameOfAnimation()){
			verschlussKickBackStarted = false;
		}
	}*/
	
	
	
	if(verschlussState == EVerschlussState::EEmptyMagKickback){
	//if(verschlussKickBackLastShotStarted){
		if(TickAnimationAndApplyLocation(
			verschlussKickBackAnimationLastShot, 
			verschlussSkeletonPointer,
			DeltaTime
		)){
			verschlussState = EVerschlussState::ENone; //lock in place, no default reset
			//verschlussKickBackAnimationLastShot.restart();
		}
		return;
	}

	if(verschlussState == EVerschlussState::EEmptyReloadRevert){
		if(TickAnimationAndApplyLocation(
			verschlussKickBackAnimationLastShotEmptyReload, 
			verschlussSkeletonPointer,
			DeltaTime
		)){
			verschlussState = EVerschlussState::ENone; //lock in place, shoot will set it
			//to default kickback automatically!
		}
		return;
	}

	if(verschlussState == EVerschlussState::EDefaultKickback){
		if(TickAnimationAndApplyLocation(
			verschlussKickBackAnimation, 
			verschlussSkeletonPointer,
			DeltaTime
		)){
			//verschlussKickBackStarted = false;
			verschlussState = EVerschlussState::ENone;
		}
		return;
	}


}


bool Aweapon::TickAnimationAndApplyLocation(
	KeyFrameAnimation &animation, 
	USkeletalMeshComponent *component,
	float DeltaTime
){
	if(component){
		FVector interpolatedLocal = animation.interpolate(DeltaTime);
		component->SetRelativeLocation(interpolatedLocal);

		bool finished = false;
		if(animation.reachedLastFrameOfAnimation()){
			finished = true;
		}
		return finished;
	}
	return true; //finished
}

/**
 * weapon sway
 */

void Aweapon::setupSwayAnimation(){
	float timeForMotion = 0.7f;
	swayInterpolator.setMotionTime(timeForMotion);
	swayInterpolator.setHeightOfWave(2.0f, -1.0f); //y,z
}

void Aweapon::updateSwayEnabledStatus(bool flag){
	if(isAiming) return;
	if(abzugHinten) return;
	
	swayInterpolator.enableSwayFlag(flag);
	
}

void Aweapon::TickSway(float deltatime){
	if(isAiming) return;
	if(abzugHinten) return;
	
	FVector interpolated = swayInterpolator.Tick(deltatime);

	//super
	Super::addToAnimationOffset(interpolated);
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
