#include "NNPathFinderExtensionApi.h"

#include "PathfinderNNExtension/Settings/NNSetting.h"

#include "PathfinderNNExtension/API/Socket/NNPathFinderSocket.h"
#include "PathfinderNNExtension/API/ONNX/NNPathFinderONNX.h"

void NNPathFinderExtensionApi::BeginPlay(UWorld *world){
    //type decided by MACRO from build.cs!
    if(NNPluginSettings::UsePythonNN()){
        ANNPathFinderSocket::MakePathFinderSocketInstance(world); //EDITOR
    }else{
        ANNPathFinderONNX::MakePathFinderONNXInstance(world); //BUILD GAME
    }
}


//predicts a node if task not qeued
void NNPathFinderExtensionApi::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    if(IPathFinderNNRequestHandleInterface *interface = TGetInstance<IPathFinderNNRequestHandleInterface>()){
        interface->PredictNode(interfaceNotify, actor);
    }
}

//notified by all entites whether the player
//is visible or not
void NNPathFinderExtensionApi::FlagVisible(AActor *actor){
    if(IPathFinderNNRequestHandleInterface *interface = TGetInstance<IPathFinderNNRequestHandleInterface>()){
        interface->FlagVisible(actor);
    }
}

#include "NNCommunicationPlugin/API/RequestTakerInterface/NNRequestTakerInterfaceBase.h"

void NNPathFinderExtensionApi::SetTrainingAllowed(){
    if(INNRequestTakerInterfaceBase *interface = TGetInstance<INNRequestTakerInterfaceBase>()){
        interface->SetTrainingAllowed();
    }
}


void NNPathFinderExtensionApi::SubscribeMessageListener(INNPathFinderSocketMessageReceiver *listener){
    if(NNPluginSettings::UsePythonNN()){
        if(ANNPathFinderSocket *socket = ANNPathFinderSocket::PathFinderNNinstance()){
            socket->SubscribeMessageListener(listener);
        }
    }
}

void NNPathFinderExtensionApi::SubscribeHeatMapListener(IPathfinderNNHeatMapReceiver *listener){
    if(IPathFinderNNRequestHandleInterface *interface = TGetInstance<IPathFinderNNRequestHandleInterface>()){
        interface->SubscribeHeatMapListener(listener);
    }
}





//use this to update the player hud ui to show the nn connection state 
//instead of using debug print.
bool NNPathFinderExtensionApi::NNIsConnected(){
    if(INNRequestTakerInterfaceBase *interface = TGetInstance<INNRequestTakerInterfaceBase>()){
        return interface->NNIsConnected();
    }
    return false;
}

void NNPathFinderExtensionApi::EnableHeatMapSaveOnEnd(bool enable){
    if(IPathFinderNNRequestHandleInterface *interface = TGetInstance<IPathFinderNNRequestHandleInterface>()){
        interface->EnableHeatMapSaveOnEnd(enable);
    }
}

bool NNPathFinderExtensionApi::HeatMapSaveOnEndEnabled(){
    if(IPathFinderNNRequestHandleInterface *interface = TGetInstance<IPathFinderNNRequestHandleInterface>()){
        return interface->HeatMapSaveOnEndEnabled();
    }
    return false;
}
