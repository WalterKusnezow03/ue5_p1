#pragma once
#include "CoreMinimal.h"

#include "NNCommunicationPlugin/Communication/Connection/base/notify/NNPathFinderSocketMessageReceiver.h"
#include "PathfinderNNExtension/API/Handle/NNRequestHandleInterface.h"
#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"

//will decide between socket and ONNX Model
//public api for all prediction tasks:
//FORKS between python and onnx.
//static interface for easier API usuage.
class PATHFINDERNNEXTENSION_API NNPathFinderExtensionApi {
    
public:
    static void BeginPlay(UWorld *world);

    //predicts a node if task not qeued
    static void PredictNode(
        IPathfinderNNInterface *interfaceNotify,
        AActor *actor
    );

    

    //notified by all entites whether the player
    //is visible or not
    static void FlagVisible(AActor *actor);

    static void SetTrainingAllowed();

    static void SubscribeMessageListener(INNPathFinderSocketMessageReceiver *listener);

    //static connection api for widgets displaying the state of 
    //connection either to python or ONNX 
    static bool NNIsConnected();

private:
    static INNRequestHandleInterface *GetInstance();

};