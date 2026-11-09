// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NNRequestTakerInterfaceBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNNRequestTakerInterfaceBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * interface for NN Any user of Request Handler (Python Socket and ONNX actor)
 */
class NNCOMMUNICATIONPLUGIN_API INNRequestTakerInterfaceBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//methods
	//virtual NNRequestHandle &GetHandler() = 0;
	//predicts a node if task not qeued
	virtual void PredictData(
        IBaseNNInterface *interfaceNotify,
        FString name,
        UObject *targetObject = nullptr
    ){};

	virtual void SetTrainingAllowed() {};


	virtual bool NNIsConnected(){
		return false;
	}

};
