// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "CoreMath/Matrix/MMatrix.h"

/**
 * 
 */


class P2_API AlertManager
{
private:
	AlertManager();
	~AlertManager();

public:
	static void EndPlay();

	static void alertInArea(UWorld *world, FVector location, float radius);

	static void damageAndAlertInArea(UWorld *world, FVector location, float SphereRadius, int damage, float damageRadius);

	static void subscribeToAlert(AEntityScript *pointer);
	static void unSubscribeFromAlert(AEntityScript *pointer);


	static void EntitiesInRadius(
		FVector &pos,
		float radius,
		TArray<FVector> &outputPositions
	);

	static bool AnyEntitesInRadius(
		FVector &pos,
		float radius
	);

	static void EntitiesInRadiusAsTransform(
		FVector &pos,
		float radius,
		TArray<MMatrix> &outputMatrices
	);


private:
	static float deltaTime(UWorld *world);

	
	/// @brief gets all entities in area, subscribed to alert manager
	/// @param world 
	/// @param location 
	/// @param radius 
	/// @return 
	static TArray<AActor *> getAActorsInArea(UWorld *world, FVector location, float radius);


	//subrciption
	static std::vector<int> subscribedActorsInAreaByIndex(const FVector &location, float SphereRadius);
	static int findIndex(AEntityScript *pointer);
	static std::vector<AEntityScript *> subscribedToAlert;


	//alert by subscription
	static void AlertSubscribedActorsInRange(const FVector &location, float SphereRadius);
	static void getDamagableActorsInAreaBySphereCast(
		UWorld *world, 
		const FVector &location, 
		float SphereRadius,
		TArray<IDamageinterface*> &outArray
	);
};
