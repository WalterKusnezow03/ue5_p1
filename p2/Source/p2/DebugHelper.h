// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class P2_API DebugHelper
{
public:
	DebugHelper();
	~DebugHelper();
	static void showScreenMessage(FString s);
	static void showScreenMessage(FString s, FColor color);
};
