#include "FCommunicationSharedFrameManager.h"
#include "NNCommunicationPlugin/Communication/Connection/base/PythonSocketBase.h"


void FCommunicationSharedFrameManager::WriteData(FString name, const TArray<uint8> &data){
    bool ignored = false;
    WriteData(name, data, ignored);
}

void FCommunicationSharedFrameManager::WriteData(
    FString pageName, 
    const TArray<uint8> &data, 
    bool &frameChanged
){
    /*DebugHelper::logMessage(
        FString::Printf(TEXT("FCommunicationSharedFrameManager page %s write"), *pageName)
    );*/
    FSharedFrameManager::WriteData(pageName, data, frameChanged);
    if(frameChanged){
        frameNameSendMap[pageName] = false;
        /*DebugHelper::logMessage(
            FString::Printf(TEXT("FCommunicationSharedFrameManager page %s changed"), *pageName)
        );*/
    }
}

void FCommunicationSharedFrameManager::ReadData(
    FString pageName, 
    TArray<uint8> &data,
    int bytesToRead
){
    bool frameChanged = false;
    FSharedFrameManager::ReadData(pageName, data, bytesToRead, frameChanged);
    if(frameChanged){
        frameNameSendMap[pageName] = false;
    }
}


void FCommunicationSharedFrameManager::MakeSureExists(FString pageName, int bytes){
    bool changed = false;
    FSharedFrameManager::MakeSureExists(pageName, bytes, changed);
    if (changed)
    {
        frameNameSendMap[pageName] = false;
    }
}




void FCommunicationSharedFrameManager::NotifyChangedFrames(APythonSocketBase *socket){
    if(socket){
        for(auto &pair : frameNameSendMap){
            FString name = pair.first;
            bool didSend = pair.second;
            if(!didSend){
                NotifyChangedFrame(name, socket);
            }
        }
    }
}

void FCommunicationSharedFrameManager::NotifyChangedFrame(
    FString name, 
    APythonSocketBase *socket
){
    if(socket){
        if(FSharedFrame *frame = FindFrame(name)){
            FString message = frame->SharedFrameIdentifierMessage("FRAMEID", name); //name as post fix tag

            socket->Send(message);
            //DebugHelper::logMessage("ANNSocket::FCommunicationSharedFrameManager:: Send frame name: ", message);
        }

        //did send.
        frameNameSendMap[name] = true;
    }
}


