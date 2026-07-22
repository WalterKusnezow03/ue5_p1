// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NNRequestHandle.h"
#include "NNRequestHandleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNNRequestHandleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface for NN Any user of Request Handler (Python Socket and ONNX actor)
 */
class PATHFINDERNNEXTENSION_API INNRequestHandleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//methods
	//virtual NNRequestHandle &GetHandler() = 0;
	//predicts a node if task not qeued
    virtual void PredictNode(
        IPathfinderNNInterface *interfaceNotify,
        AActor *actor
    ){};

    

    //notified by all entites whether the player
    //is visible or not
	virtual void FlagVisible(AActor *actor) {};

	virtual void SetTrainingAllowed() {};


	virtual bool NNIsConnected(){
		return false;
	}
};
