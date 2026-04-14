// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "p2/entities/EntityScript.h"
#include "CoreMath/Matrix/MMatrix.h"

#include "AlertManager.generated.h"

/**
 * 
 */

UCLASS()
class P2_API AAlertManager : public AActor
{
	GENERATED_BODY()
private:
	static AAlertManager *instancePtr;

public:
	static AAlertManager *Instance(UWorld *world);

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void EndPlayClear();

	void alertInArea(FVector location, float radius);

	void damageAndAlertInArea(UWorld *world, FVector location, float SphereRadius, int damage, float damageRadius);

	void subscribeToAlert(AEntityScript *pointer);
	void unSubscribeFromAlert(AEntityScript *pointer);


	void EntitiesInRadius(
		FVector &pos,
		float radius,
		TArray<FVector> &outputPositions
	);


	void EntitiesInRadiusFootPositions(
		FVector &pos,
		float radius,
		TArray<FVector> &outputPositions
	);

	bool AnyEntitesInRadius(
		FVector &pos,
		float radius
	);

	void EntitiesInRadiusAsTransform(
		FVector &pos,
		float radius,
		TArray<MMatrix> &outputMatrices
	);


private:
	

	
	/// @brief gets all entities in area, subscribed to alert manager
	/// @param world 
	/// @param location 
	/// @param radius 
	/// @return 
 	TArray<AActor *> getAActorsInArea(UWorld *world, FVector location, float radius);


	//subrciption
	std::vector<int> subscribedActorsInAreaByIndex(const FVector &location, float SphereRadius);
	int findIndex(AEntityScript *pointer);
	std::vector<AEntityScript *> subscribedToAlert;


	//alert by subscription
	void AlertSubscribedActorsInRange(const FVector &location, float SphereRadius);
	void getDamagableActorsInAreaBySphereCast(
		UWorld *world, 
		const FVector &location, 
		float SphereRadius,
		TArray<IDamageinterface*> &outArray
	);





	TArray<AEntityScript*> EntitiesInRadiusProtected(
		FVector &pos,
		float radius
	);
};
