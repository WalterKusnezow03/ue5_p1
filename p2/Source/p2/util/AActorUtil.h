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
	static void findAllComponentsByType(AActor &actor, std::vector<T *> container);

	static void findDirectChildsByName(AActor &actor, FString namerfac, TArray<UChildActorComponent *> container);

private:
	AActorUtil();
	~AActorUtil();
};
