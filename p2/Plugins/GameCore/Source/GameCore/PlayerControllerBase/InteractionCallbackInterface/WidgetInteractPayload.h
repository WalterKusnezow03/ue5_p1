// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WidgetInteractPayload.generated.h"

/// @brief empty payload class for widget interactions.
UCLASS()
class GAMECORE_API UWidgetInteractPayload : public UObject{
	GENERATED_BODY()

public:


	//might have an aactor
	void SetActor(AActor *inActor);
	AActor *GetActor() const;

protected:
	AActor *actor = nullptr;
};