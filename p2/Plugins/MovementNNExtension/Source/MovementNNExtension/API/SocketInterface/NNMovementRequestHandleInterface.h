// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "MovementNNExtension/API/Handle/NotifyInterface/NNMovementInterface.h"
#include "NNMovementRequestHandleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNNMovementRequestHandleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * secondary interface for NN Any user of Request Handler (Python Socket and ONNX actor)
 */
class MOVEMENTNNEXTENSION_API INNMovementRequestHandleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//methods
	//virtual NNRequestHandle &GetHandler() = 0;
	//predicts a node if task not qeued
    virtual void PredictTrajectories(
        INNMovementInterface *interfaceNotify,
        AActor *actor
    ){};

};
