// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"
#include "PathfinderNNExtension/Interface/HeatMapReceiverCollection/HeatMapReceivers.h"
#include "PathFinderNNRequestHandleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPathFinderNNRequestHandleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * secondary interface for NN Any user of Request Handler (Python Socket and ONNX actor)
 */
class PATHFINDERNNEXTENSION_API IPathFinderNNRequestHandleInterface
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


	//disable inside request habdle
	virtual void EnableHeatMapSaveOnEnd(bool saveOnEnd) {};
	virtual bool HeatMapSaveOnEndEnabled(){
		return false;
	}


	//widget display of heatmap / other sample type
	void SubscribeHeatMapListener(IPathfinderNNHeatMapReceiver *listener){
		if(listener){
			GetHeatMapReceivers().Subscribe(listener);
		}
	}
	
protected:
	virtual HeatMapReceivers &GetHeatMapReceivers() = 0;
};
