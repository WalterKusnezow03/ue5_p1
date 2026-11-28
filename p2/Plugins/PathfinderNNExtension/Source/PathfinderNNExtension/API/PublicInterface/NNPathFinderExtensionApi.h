#pragma once
#include "CoreMinimal.h"

#include "NNCommunicationPlugin/Communication/Connection/base/notify/NNSocketMessageReceiver.h"
#include "PathfinderNNExtension/API/Handle/PathFinderNNRequestHandleInterface.h"
#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"
#include "NNCommunicationPlugin/Settings/NNSetting.h"
#include "PathfinderNNExtension/API/Socket/NNPathFinderSocket.h"
#include "PathfinderNNExtension/API/ONNX/NNPathFinderONNX.h"

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

    static void SubscribeMessageListener(INNSocketMessageReceiver *listener);
    
    static void SubscribeHeatMapListener(IPathfinderNNHeatMapReceiver *listener);

    //static connection api for widgets displaying the state of 
    //connection either to python or ONNX 
    static bool NNIsConnected();
    //static image generation of heatmap for minimap
    

    //static heatmap enable (for onnx output and python output)
    static void EnableHeatMapSaveOnEnd(bool enable);
    static bool HeatMapSaveOnEndEnabled();

protected:
    template<typename T>
    static T *TGetInstance(){
        AActor *instanceFound = nullptr;
        if(NNPluginSettings::UsePythonNN()){
            instanceFound = ANNPathFinderSocket::PathFinderNNinstance();
        }else{
            instanceFound = ANNPathFinderONNX::ApiInstance();
        }
        if(instanceFound){
            return Cast<T>(instanceFound);
        }
        return nullptr;
    }

private:
    static IPathFinderNNRequestHandleInterface *GetInstance();

};