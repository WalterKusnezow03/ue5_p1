#include "MessageDispatcher.h"
#include "DebugPlugin/DebugHelper.h"

MessageDispatcher::MessageDispatcher(){

}

MessageDispatcher::~MessageDispatcher(){

}

void MessageDispatcher::Subscribe(INNPathFinderSocketMessageReceiver *other){
    if(other != nullptr){
        //has
        if(!receiverSet.Contains(other)){
            receiverSet.Add(other);
            DebugHelper::logMessage("Python::MessageDispatcher Subscribe!");
        }
    }
}


void MessageDispatcher::Dispatch(FString message){
    if(!message.ToLower().Contains("debug")){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Python::MessageDispatcher::Dispatch (TO %d) %s"),
                receiverSet.Num(),
                *message
            )
        );
    }
    
    for (int i = 0; i < receiverSet.Num(); i++){
        if(INNPathFinderSocketMessageReceiver *current = receiverSet[i]){
            current->ReceiveMessage(message);
        }
    }
}