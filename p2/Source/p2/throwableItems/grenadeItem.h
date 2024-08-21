// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/throwableItems/throwableItem.h"
#include "p2/util/timer.h"
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

	class timer timer;

	static const int EXPLOSION_RADIUS = 10000; //20 000 --> (20 * 100 = 20m)
	static const int DAMAGE = 60;

	bool isDetonated;
};
