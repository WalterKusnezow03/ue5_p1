// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "WidgetInteractionCallbackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetInteractionCallbackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface for calling back aactor on widget interaction - for example after timer.
 */
class IWidgetInteractionCallbackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void ReceiveCallback() {}; //empty callback
    virtual void ReceiveCallback(AActor *payloadActor){}; //for now just aactor
};
