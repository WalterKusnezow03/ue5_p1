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

//todo here: close page
void FCommunicationSharedFrameManager::ClosePage(FString pageName){
    //if the close was successfull python must be notified, track change!
    if(FSharedFrameManager::Close(pageName)){
        //mark notify needed
        frameNameCloseSendMap[pageName] = false;
    }
}



void FCommunicationSharedFrameManager::NotifyChangedFrames(APythonSocketBase *socket){
    if(socket){
        
        //notify opened frames
        NotifyChangedFrames(socket, frameNameSendMap, "FRAMEID");
        NotifyChangedFrames(socket, frameNameCloseSendMap, "FRAMEIDCLOSE");


        // notify closed frames

        // ---- deprecated ----
        /*
        //notify opened frames
        for(auto &pair : frameNameSendMap){
            FString name = pair.first;
            bool didSend = pair.second;
            if(!didSend){
                NotifyChangedFrame(name, socket);
            }
        }
        */
    }
}

//new - testing needed !
void FCommunicationSharedFrameManager::NotifyChangedFrames(
    APythonSocketBase *socket,
    std::map<FString, bool> &mapToMessage,
    FString prefixCommandForPython //might be "FRAMEID" for open, or "FRAMEIDCLOSE" for closing the shared memory
){
    if(socket){
        for(auto &pair : mapToMessage){
            FString name = pair.first;
            bool didSend = pair.second;
            if(!didSend){
                NotifyChangedFrame(socket, mapToMessage, name, prefixCommandForPython);
            }
        }
    }
}

//new - testing needed 
void FCommunicationSharedFrameManager::NotifyChangedFrame(
    APythonSocketBase *socket,
    std::map<FString, bool> &mapToMessage,
    FString name, //shared frame name
    FString prefixCommandForPython //might be "FRAMEID" for open, or "FRAMEIDCLOSE" for closing the shared memory
){
    if(socket){
        if(FSharedFrame *frame = FindFrame(name)){
        
            // --- INSIDE PYTHON PSEUDO CODE ---
            //messages for python like this:
            //prefixCommand = message[0]
            //tagname, size, shortTag = message[1], int(message[2]), message[3]
            //"prefix_frameIdentifierTag_Size_shortTag
            //example: message = frame->SharedFrameIdentifierMessage("FRAMEID", name); 
            FString message = frame->SharedFrameIdentifierMessage(prefixCommandForPython, name); 

            socket->Send(message);
            //DebugHelper::logMessage("ANNSocket::FCommunicationSharedFrameManager:: Send frame name: ", message);
        }

        //did send.
        //frameNameSendMap[name] = true;
        mapToMessage[name] = true; //update was send must be flagged to true.
    }
}


//DEPRECATED!
/*
void FCommunicationSharedFrameManager::NotifyChangedFrame(
    FString name, 
    APythonSocketBase *socket
){
    if(socket){
        if(FSharedFrame *frame = FindFrame(name)){

            // --- INSIDE PYTHON PSEUDO CODE ---
            //messages for python like this:
            //prefixCommand = message[0]
            //tagname, size, shortTag = message[1], int(message[2]), message[3]
            //"prefix_frameIdentifierTag_Size_shortTag
            FString message = frame->SharedFrameIdentifierMessage("FRAMEID", name); //name as post fix tag

            socket->Send(message);
            //DebugHelper::logMessage("ANNSocket::FCommunicationSharedFrameManager:: Send frame name: ", message);
        }

        //did send.
        frameNameSendMap[name] = true;
    }
}
*/

//todo: FRAMEIDCLOSE

