// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "sightScript.h"
#include "carriedItem.h"
#include "weaponEnum.h"
#include "ammunitionEnum.h"
#include "attachmentEnums/weaponSightEnum.h"
#include "p2/player/teamEnum.h"

#include "weapon.generated.h"

UCLASS()
class P2_API Aweapon : public AcarriedItem
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties (constructor)
	Aweapon();

	// Enum to specify the type of weapon -> is saved to create animation paths
	// enum values will resemble their EXACT type for animation locations in folder structure!
	// DO NOT REMOVE
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	weaponEnum Type;

	bool DEBUG_DRAW = false;

public:
	weaponEnum readType();

	virtual void showWeapon(bool show);
	/*
	void pickup(UCameraComponent &cameraRefIn);
	void pickupBot(AActor *actorIn); //pickup for bot!
	*/
	//void dropweapon();
	virtual void drop() override;
	bool isPickedup();
	virtual void reload(int amount); //might be override from other classes (for example thrower weapon)
	int getMagSize();
	int getBulletsInMag();

	//the class has 3 shoot methods: both public methods collect start and target point
	//the "shootProtected" must only be called within the class!
	virtual void shoot(); //PLAYER SHOOT METHOD --> can be overriden for subclasses
	virtual void shootBot(FVector target); //BOT SHOOT METHOD --> can be overriden for subclasses

	void releaseShoot();
	void aim(bool aimstatus);

	bool isCooling();
	bool enoughBulletsInMag();
	bool canShoot();
	bool canReload();

	float recoilValue();

	//returns if the weapon is active or not
	//bool isActive();

	virtual FVector leftHandLocation() override;
	virtual FVector rightHandLocation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/// @brief will save whether the weapon is shown (selected or not. Blocks shooting)
	//bool isVisible;

	//this shoot method is PROTECTED against the outside, only use shoot or shootBot
	virtual void shootProtected(FVector from, FVector to, teamEnum ownTeam);

	bool singleFireModeOn;
	bool singleFireMode();

	/**
	 * must be resetet if not a single fire weapon when cooldown complete
	 */
	bool abzugHinten;
	bool isAiming;
	int bulletsInMag;

	float timeleft;
	float cooldownTime;
	float reloadTime;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//void followPlayer();
	void updateCooltime(float time);
	void resetCoolTime(float time);
	float calculateRpm(int rpm);
	void setupSight();

	/// @brief offset vector if any sight is attached, also: hipfire adjust
	/// @return vector to add to actor location
	virtual FVector getOffsetVector() override;

	//saves the current sight attachment
	class AsightScript *sightPointer;

	//animations
	void shootAnimation();
	void reloadAnimation();

	void animationPathSet();
	void setVerschlussPath(FString path);
	void setMagAnimPath(FString path);
	void setupAnimations();

	//plays any anim
	//void playAnimation(const FString &AnimationPath, USkeletalMeshComponent *skeleton, float time);
	void playAnimation(UAnimSequence *AnimSequence, USkeletalMeshComponent *skeleton, float time);

	class USkeletalMeshComponent *rightHandTargetSkelletonPointer;
	class USkeletalMeshComponent *leftHandTargetSkelletonPointer;

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
	
	/// @brief saves all sights of the weapon find from actor to enable disable them by selected type
	std::map<weaponSightEnum, AActor *> sightMap;

public:
	void applySight(weaponSightEnum sight);

	//get ammuntion type for this weapon
	virtual ammunitionEnum getAmmunitionType();
};
