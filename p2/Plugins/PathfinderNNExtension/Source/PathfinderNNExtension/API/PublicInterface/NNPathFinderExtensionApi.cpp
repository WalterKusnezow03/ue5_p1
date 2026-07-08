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

INNRequestHandleInterface *NNPathFinderExtensionApi::GetInstance(){
    if(NNPluginSettings::UsePythonNN()){
        return ANNPathFinderSocket::PathFinderNNinstance();
    }else{
        return ANNPathFinderONNX::ApiInstance();
    }
    return nullptr;
}

//predicts a node if task not qeued
void NNPathFinderExtensionApi::PredictNode(
    IPathfinderNNInterface *interfaceNotify,
    AActor *actor
){
    if(INNRequestHandleInterface *interface = GetInstance()){
        interface->PredictNode(interfaceNotify, actor);
    }
}

//notified by all entites whether the player
//is visible or not
void NNPathFinderExtensionApi::FlagVisible(AActor *actor){
    if(INNRequestHandleInterface *interface = GetInstance()){
        interface->FlagVisible(actor);
    }
}

void NNPathFinderExtensionApi::SetTrainingAllowed(){
    if(INNRequestHandleInterface *interface = GetInstance()){
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
