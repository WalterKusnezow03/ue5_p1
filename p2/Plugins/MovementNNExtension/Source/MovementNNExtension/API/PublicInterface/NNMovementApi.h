#pragma once
#include "CoreMinimal.h"

#include "NNCommunicationPlugin/Communication/Connection/base/notify/NNSocketMessageReceiver.h"

#include "NNCommunicationPlugin/Settings/NNSetting.h"

#include "MovementNNExtension/API/ONNX/NNMovementONNX.h"
#include "MovementNNExtension/API/Socket/NNMovementSocket.h"
#include "MovementNNExtension/API/SocketInterface/NNMovementRequestHandleInterface.h"

//will decide between socket and ONNX Model
//public api for all prediction tasks:
//FORKS between python and onnx.
//static interface for easier API usuage.
class MOVEMENTNNEXTENSION_API NNMovementApi {
    
public:
    static void BeginPlay(UWorld *world);

    //predicts a node if task not qeued
    static void PredictTrajectories(
        INNMovementInterface *interfaceNotify,
        AActor *actor
    );

    static void SetTrainingAllowed();

    static void SubscribeMessageListener(INNSocketMessageReceiver *listener);
    

    //static connection api for widgets displaying the state of 
    //connection either to python or ONNX 
    static bool NNIsConnected();

    static void SetGameStatePaused(bool state);

protected:
    template<typename T>
    static T *TGetInstance(){
        AActor *instanceFound = nullptr;
        if(NNPluginSettings::UsePythonNN()){
            instanceFound = ANNMovementSocket::NNMovementinstance();
        }else{
            instanceFound = ANNMovementONNX::ApiInstance();
        }
        if(instanceFound){
            return Cast<T>(instanceFound);
        }
        return nullptr;
    }



};