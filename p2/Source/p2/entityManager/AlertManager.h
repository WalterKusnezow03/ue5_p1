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

private:
	static float deltaTime(UWorld *world);
};
