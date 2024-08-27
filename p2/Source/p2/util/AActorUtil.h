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


	//show and hide
	static void showActor(AActor &actor, bool show);
	static void showChildActorComponent(UChildActorComponent &component, bool show);

private:
	AActorUtil();
	~AActorUtil();
};
