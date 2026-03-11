// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "sightScript.h"
#include "p2/weapon/CarriedItemBase/carriedItem.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "ammunitionEnum.h"
#include "GameCore/team/teamEnum.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "CoreMath/animation/KeyFrames/KeyFrameAnimation.h"
#include "CoreMath/animation/timer/Timer.h"
#include "CoreMath/animation/timer/PayloadTimer.h"
#include "p2/weapon/sway/SwayInterpolator.h"
#include "GameCore/interfaces/DamageInterface/CustomHitResult.h"

#include "weapon.generated.h"





//forward declare
class IDamageinterface;

UCLASS()
class P2_API Aweapon : public AcarriedItem
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties (constructor)
	Aweapon();

	weaponEnum weaponType();
	virtual ammunitionEnum getAmmunitionType();

	// Enum to specify the type of weapon -> is saved to create animation paths
	// enum values will resemble their EXACT type for animation locations in folder structure!
	// DO NOT REMOVE
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	weaponEnum Type;

	bool DEBUG_DRAW = false;

public:
	
	//Override from IKCarrayInterface: accumulate sight offset!
	virtual void UpdateActorTransform(FVector &location, FRotator &rotation) override;

	
	virtual EMarkerType GetMarkerType() override {
        return EMarkerType::EWeapon;
    }
    



	virtual void showWeapon(bool show);
	/*
	void pickup(UCameraComponent &cameraRefIn);
	void pickupBot(AActor *actorIn); //pickup for bot!
	*/
	//void dropweapon();
	virtual void drop() override;
	void dropToObjectPool();
	void resetFlags();
	bool isPickedup();
	virtual void reload(int amount); //might be override from other classes (for example thrower weapon)
	int getMagSize();
	int getBulletsInMag();



	//the class has 3 shoot methods: both public methods collect start and target point
	//the "shootProtected" must only be called within the class!
	virtual void shoot(); //PLAYER SHOOT METHOD --> can be overriden for subclasses
	virtual void shootBot(FVector target); //BOT SHOOT METHOD --> can be overriden for subclasses

	void releaseShoot();
	virtual void aim(bool aimstatus);

	bool isCooling();
	bool enoughBulletsInMag();
	bool canShoot();
	bool canReload();
	virtual bool isSoundSurpressed();

	float recoilValue();

	//returns if the weapon is active or not
	//bool isActive();



private:
	


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/// @brief will save whether the weapon is shown (selected or not. Blocks shooting)
	//bool isVisible;

	//this shoot method is PROTECTED against the outside, only use shoot or shootBot
	//is marked as bool to flag if a new hitresult was made.
	//returns true if any actor was hit. HitResult is updated!
	virtual bool shootProtected(FVector from, FVector dir, float sizeRay, teamEnum ownTeam);

	//this shoot method is PROTECTED against the outside, only use shoot or shootBot
	//is marked as bool to flag if a new hitresult was made.
	//returns true if any actor was hit. HitResult is updated!
	//bool damageOnHit to apply damage to actor, might be disabled for any reason!
	virtual bool shootProtected(FVector Start, FVector dir, float sizeRay, teamEnum ownTeam, bool damageOnHit);

	//-- latest raycast hit tracking -- 
	FHitResult latestHit;
	FVector LatestHitLocation();
	FVector LatestHitNormal();



	FCustomHitResult MakeHitResult(
		FHitResult &HitResult, 
		FVector &start
	);


	// -- damage propagation on hit --
	void damageIfPossible(
		teamEnum ownTeam,
		AActor *actor,
		FHitResult &hitresult,
		FVector &start,
		FVector &dir,
		float sizeRay
	);

	void damageIfPossible(
		teamEnum ownTeam,
		AActor *entity,
		FHitResult &hitresult,
		FVector &start,
		FVector &end
	);

	void damageIfPossible(
		teamEnum ownTeam,
		IDamageinterface *entity,
		FHitResult &hitresult,
		FVector &start,
		FVector &end
	);

	bool singleFireModeOn;
	bool singleFireMode();

	/**
	 * must be resetet if not a single fire weapon when cooldown complete
	 */
	bool abzugHinten;
	bool isAiming;
	int bulletsInMag;
	bool isReloading = false;

	
	float cooldownTime();
	float reloadTime();

	class PayloadTimer timer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//void followPlayer();
	void updateCooltime(float time);
	void resetCoolTime(float time);
	float calculateRpm(int rpm);
	

	/// @brief offset vector if any sight is attached, also: hipfire adjust
	/// @return vector to add to actor location
	virtual FVector getOffsetVector() override;

	//saves the current sight attachment
	class AsightScript *sightPointer;
	class AActor *sightActorPointer;

	//animations
	void shootAnimation();
	void reloadAnimation();

	
	
	void setupAnimations();
	void LoadAnimationsFromAssetManager();

	//plays any anim
	//void playAnimation(const FString &AnimationPath, USkeletalMeshComponent *skeleton, float time);
	void playAnimation(UAnimSequence *AnimSequence, USkeletalMeshComponent *skeleton, float time);

	class USkeletalMeshComponent *rightHandTargetSkelletonPointer;
	class USkeletalMeshComponent *leftHandTargetSkelletonPointer;
	class USkeletalMeshComponent *muzzleAttachmentSkelletonPointer;
	class USkeletalMeshComponent *gripAttachmentSkelletonPointer;

	class USkeletalMeshComponent *verschlussSkeletonPointer;
	class USkeletalMeshComponent *magSkeletonPointer;
	class USkeletalMeshComponent *gehauseSkeletonPointer;
	FString verschlussPath;
	FString magAnimPath;
	FString gehauseAnimPath;

	//saves the preloaded animation sequences
	class UAnimSequence *verschlussAnimationSquence;
	class UAnimSequence *gehauseAnimSequence;
	class UAnimSequence *magAnimationSequence;
	class UAnimSequence *magAnimationShootSequence;
	FString magShootAnimpath;

	//find attachments
	void findAttachmentChildActors();

	//attachments pointers 
	class UChildActorComponent *reddotSightChildActor;
	class UChildActorComponent *ironSightChildActor;
	
	/// @brief saves all sights of the weapon to spawn to enable disable them by selected type
	std::map<weaponAttachmentEnum, AActor *> sightMap;

	/// @brief saves all muzzle of the weapon to spawn to enable disable them by selected type
	std::map<weaponAttachmentEnum, AActor *> muzzleMap;

	/// @brief saves all grips of the weapon to spawn to enable disable them by selected type
	std::map<weaponAttachmentEnum, AActor *> gripMap;

	weaponAttachmentEnum pickedSight;
	weaponAttachmentEnum pickedMuzzle;
	weaponAttachmentEnum pickedGrip;
	

	int damageForAmmunitionType();

public:
	void applySight(weaponAttachmentEnum sight);
	void applyMuzzle(weaponAttachmentEnum muzzle);
	void applyGrip(weaponAttachmentEnum type);

	

	void updateSwayEnabledStatus(bool flag);



	/**
	 * 
	 * -- new expiremental custom attachment section --
	 * 
	 */
private:
	void spawnAllAvailableAttachments();
	void loadAndSaveAttachment(weaponAttachmentEnum EattachmentType);
	void attachNewItem(AActor *someActor);
	void attachNewItem(AActor *actor, weaponAttachmentEnum type);
	USkeletalMeshComponent *attachmentSkeletalComponentBy(weaponAttachmentEnum type);

	void showAllPickedAttachments();
	void hideAllAttachments();
	void hideAllAttachments(std::map<weaponAttachmentEnum, AActor *> &map);



protected:
	bool actorAlreadyAttached(AActor *actorpointer);
	std::vector<AActor *> attachedActors;

	AActor *findFromMapAndEnable(
		std::map<weaponAttachmentEnum, AActor *> &map,
		weaponAttachmentEnum type
	);

	//new implementing animation from bonecontroller class
	class KeyFrameAnimation actorKickBackAnim;
	void flagKickbackStart();
	bool kickbackStarted = false;
	bool verschlussKickBackStarted = false;
	bool recoilCopied = false;

	//on shoot (will be replaced by placing animation in placable item)
	virtual void setupKickBackAnimation();
	void ClearKickbackAnimation();

	bool kickbackIsRunning();
	void TickKickback(float DeltaTime);

	class KeyFrameAnimation verschlussKickBackAnimation;
	void setupVerschlussAnimation();
	void TickVerschlussKickBack(float DeltaTime);

	//sway
	SwayInterpolator swayInterpolator;
	void setupSwayAnimation();
	void TickSway(float deltatime);
	


	//sight offset
	void findSightOffset();
	void resetSightOffset();
};
