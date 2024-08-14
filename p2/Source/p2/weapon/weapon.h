// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "sightScript.h"
#include "carriedItem.h"
#include "weaponEnum.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	weaponEnum Type;


	void showWeapon(bool show);
	/*
	void pickup(UCameraComponent &cameraRefIn);
	void pickupBot(AActor *actorIn); //pickup for bot!
	*/
	void dropweapon();
	virtual void drop() override;
	bool isPickedup();
	void reload(int amount);
	int getMagSize();
	int getBulletsInMag();

	//the class has 3 shoot methods: both public methods collect start and target point
	//the "shootProtected" must only be called within the class!
	void shoot(); //PLAYER SHOOT METHOD
	void shootBot(FVector target); //BOT SHOOT METHOD

	void releaseShoot();
	void aim(bool aimstatus);

	bool isCooling();
	bool enoughBulletsInMag();
	bool canShoot();
	bool canReload();

	float recoilValue();

	//returns if the weapon is active or not
	//bool isActive();

protected:
	/// @brief will save whether the weapon is shown (selected or not. Blocks shooting)
	//bool isVisible;

	//this shoot method is PROTECTED against the outside, only use shoot or shootBot
	virtual void shootProtected(FVector from, FVector to);

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
	void playAnimation(const FString &AnimationPath, USkeletalMeshComponent *skeleton, float time);

	class USkeletalMeshComponent *verschlussSkeletonPointer;
	class USkeletalMeshComponent *magSkeletonPointer;
	FString verschlussPath;
	FString magAnimPath;



	//follow
	/*class UCameraComponent *cameraPointer; // why class, idk, just keep it like that
	class AActor *botPointer;

	FVector offset;

	void showScreenMessage(FString s);

	void enableCollider(bool enable);
	*/



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
    



	
};
