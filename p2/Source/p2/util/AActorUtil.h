// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API AActorUtil
{
public:
	template <typename T>
	static void findAllComponentsByType(AActor &actor, std::vector<T *> &container); //container passed by ref obviously

	static void findDirectChildsByName(AActor &actor, FString namerfac, TArray<UChildActorComponent *> &container);

	static void findAllChildsByName(AActor &actor, FString namepart, TArray<UChildActorComponent *> &container);

	//show and hide
	static void showActor(AActor &actor, bool show);
	static void showChildActorComponent(UChildActorComponent &component, bool show);

	//collision
	static void enableColliderOnActor(AActor &actor, bool enable);

	//calulate bounds
	static void calculateActorBounds(AActor *actor, int &x, int &y, int &z);
	static void calculateActorBounds(AActor *actor, int &x, int &y, int &z, FVector &origin, FVector &extent);
	static void calculateActorBounds(UWorld *world, UClass *uclassIn, int &x, int &y, int &z);

private:
	AActorUtil();
	~AActorUtil();
};
