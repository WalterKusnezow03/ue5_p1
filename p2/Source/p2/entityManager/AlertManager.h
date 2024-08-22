// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API AlertManager
{
public:
	AlertManager();
	~AlertManager();

	static void alertInArea(UWorld *world, FVector location, float radius);

	static void damageAndAlertInArea(UWorld *world, FVector location, float SphereRadius, int damage, float damageRadius);

private:
	static float deltaTime(UWorld *world);

	static TArray<AActor *> getAActorsInArea(UWorld *world, FVector location, float radius);
};
