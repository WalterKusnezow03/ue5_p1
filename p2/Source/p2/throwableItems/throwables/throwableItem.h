// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/throwableItems/base/LaunchableItemBase.h"
#include "p2/weapon/throwerWeapon/enum/throwableEnum.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "throwableItem.generated.h"

UCLASS()
class P2_API AthrowableItem : public ALaunchableItemBase
{
	GENERATED_BODY()
	
public:	
	

	
	virtual void throwIntoDirection(FVector start, FVector direction);

	virtual void reset() override;
	

protected:
	
	bool bPhysicsEnabled = false;

	virtual void release() override;
	void AddImpulse(const FVector &direction, float velocity);

	void enablePhysics(bool enable);

	bool PhysicsEnabled();
	float Velocity();

	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
