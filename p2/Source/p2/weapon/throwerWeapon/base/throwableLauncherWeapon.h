// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/weapon.h"
#include "p2/throwableItems/base/LaunchableItemBase.h"
#include "GameCore/team/teamEnum.h"
#include "throwableLauncherWeapon.generated.h"

/**
 * LAUNCHER FOR THROWABLE ITEMS - NOT WEAPONS!
 * THROWABLE ITEM SPAWNER AND EJECTER!
 * 
 * Could be a: RPG launcher, a bow with arrows.
 */
UCLASS()
class P2_API AthrowableLauncherWeapon : public Aweapon
{
	GENERATED_BODY()

public:
	AthrowableLauncherWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void showWeapon(bool show) override;

	void setThrowableType(EThrowableEnum type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "throwabletype")
	EThrowableEnum throwableTypeToThrow = EThrowableEnum::Enone; //type to throw / instantiate

protected:
	UPROPERTY()
	USceneComponent *socketForThrowable = nullptr;
	void FindThrowableSocketOnBeginPlay();
	void FindDefaultThrowableOnBeginPlay();

	virtual bool shootProtected(FVector from, FVector dir, float sizeRay, teamEnum ownTeam) override;
	virtual void reload(int amount) override;

	bool showThrowableItem;
	ALaunchableItemBase *launchableActorPointer;


	virtual void RequestNewThrowableIfNeeded();

	void AttachLaunchableActor(ALaunchableItemBase *actor);
	void detachLaunchableActorPointer();

public:
	//get ammuntion type for this weapon, overriden for thrower weapons to return their throwable
	virtual ammunitionEnum getAmmunitionType() override;
	
};
