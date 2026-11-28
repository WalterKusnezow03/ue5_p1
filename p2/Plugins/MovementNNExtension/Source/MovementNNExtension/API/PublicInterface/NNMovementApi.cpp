#include "NNMovementApi.h"

#include "NNCommunicationPlugin/Settings/NNSetting.h"
#include "MovementNNExtension/API/Handle/NotifyInterface/NNMovementInterface.h"

void NNMovementApi::BeginPlay(UWorld *world){
    //type decided by MACRO from build.cs!
    if(NNPluginSettings::UsePythonNN()){
        ANNMovementSocket::MakeNNMovementSocketInstance(world); //EDITOR
    }else{
        ANNMovementONNX::MakeMovementONNXInstance(world); //BUILD GAME
    }
}


//predicts a node if task not qeued
void NNMovementApi::PredictTrajectories(
    INNMovementInterface *interfaceNotify,
    AActor *actor
){
    if(INNMovementRequestHandleInterface *interface = TGetInstance<INNMovementRequestHandleInterface>()){
        interface->PredictTrajectories(interfaceNotify, actor);
    }
}


#include "NNCommunicationPlugin/API/RequestTakerInterface/NNRequestTakerInterfaceBase.h"

void NNMovementApi::SetTrainingAllowed(){
    DebugHelper::logMessage("NNMovementApi::SetTrainingAllowed");
    if(INNRequestTakerInterfaceBase *interface = TGetInstance<INNRequestTakerInterfaceBase>()){
        interface->SetTrainingAllowed();
    }
}

void NNMovementApi::SetGameStatePaused(bool state){
    if(INNRequestTakerInterfaceBase *interface = TGetInstance<INNRequestTakerInterfaceBase>()){
        interface->SetGameStatePaused(state);
    }
}


void NNMovementApi::SubscribeMessageListener(INNSocketMessageReceiver *listener){
    if(NNPluginSettings::UsePythonNN()){
        if(ANNMovementSocket *socket = ANNMovementSocket::NNMovementinstance()){
            socket->SubscribeMessageListener(listener);
        }
    }
}





//use this to update the player hud ui to show the nn connection state 
//instead of using debug print.
bool NNMovementApi::NNIsConnected(){
    if(INNRequestTakerInterfaceBase *interface = TGetInstance<INNRequestTakerInterfaceBase>()){
        return interface->NNIsConnected();
    }
    return false;
}



