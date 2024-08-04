// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "sightScript.h"

#include "weapon.generated.h"

UCLASS()
class P2_API Aweapon : public AActor
{
	GENERATED_BODY()
	
public:

	

	// Sets default values for this actor's properties (constructor)
	Aweapon();

	void setupSight();
	void pickup(UCameraComponent &cameraRefIn);
	void dropweapon();
	bool isPickedup();
	void reload(int amount);
	void shoot(FVector from, FVector to);
	void shoot(); //with default camera which is passed in by then
	void releaseShoot();
	void aim(bool aimstatus);

	bool isCooling();
	bool enoughBulletsInMag();
	bool canShoot();

protected:
	/**
	 * must be resetet if not a single fire weapon when cooldown complete
	 */
	bool abzugHinten;
	bool isAiming;
	int bulletsInMag;

	float timeleft;
	float cooldownTime;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void followPlayer();
	void updateCooltime(float time);
	void resetCoolTime(float time);
	float calculateRpm(int rpm);

	FVector getOffsetVector();

	//saves the current sight attachment
	class AsightScript *sightPointer;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
    



private:
	class UCameraComponent *cameraPointer; //why class, idk, just keep it like that

	FVector offset;

	void showScreenMessage(FString s);
};
