// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/throwableItems/throwableItem.h"
#include "CoreMath/animation/timer/Timer.h"
#include "p2/weapon/ExplosiveHelper/ExplosiveHelper.h"
#include "grenadeItem.generated.h"

/**
 * 
 */
UCLASS()
class P2_API AgrenadeItem : public AthrowableItem
{
	GENERATED_BODY()

public:
	AgrenadeItem();

	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void throwIntoDirection(FVector start, FVector direction) override;

	void detonate();

	class Timer timer;

	ExplosiveHelper explosiveHelper;
	int EXPLOSION_RADIUS = 10000; // 20 000 --> (20 * 100 = 20m)
	int DAMAGE = 40;
	int DAMAGE_RADIUS = 5000;

	bool isDetonated;
};
