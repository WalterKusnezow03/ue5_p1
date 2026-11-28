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
        
        //notify opened frames - wait for approval / mirror true
        NotifyChangedFrames(socket, frameNameSendMap, "FRAMEID", true);
        
        //notify once
        NotifyChangedFrames(socket, frameNameCloseSendMap, "FRAMEIDCLOSE", false);


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
    FString prefixCommandForPython, //might be "FRAMEID" for open, or "FRAMEIDCLOSE" for closing the shared memory
    bool waitForMirror 
){
    if(socket){
        for(auto &pair : mapToMessage){
            FString name = pair.first;
            bool didSend = pair.second;
            if(!didSend){
                NotifyChangedFrame(socket, mapToMessage, name, prefixCommandForPython, waitForMirror);
            }
        }
    }
}

//new - testing needed 
void FCommunicationSharedFrameManager::NotifyChangedFrame(
    APythonSocketBase *socket,
    std::map<FString, bool> &mapToMessage,
    FString name, //shared frame name
    FString prefixCommandForPython, //might be "FRAMEID" for open, or "FRAMEIDCLOSE" for closing the shared memory
    bool waitForMirror 
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
            DebugHelper::logMessage("FCommunicationSharedFrameManager:: Send frame name: ", message);
        }

        /** DEPRECATED, WAIT FOR MIRROR */
        //did send.
        //mapToMessage[name] = true; //update was send must be flagged to true.

        if(!waitForMirror){
            //did send.
            mapToMessage[name] = true; //update was send must be flagged to true.
        }
    }
}


void FCommunicationSharedFrameManager::NotifyMessageMirror(FString message){
    FString cleaned;
    if(!IsSharedFrameIdentifierMessage(message, cleaned)){
        return;
    }

    if(cleaned.StartsWith(TEXT("FRAMEID_"), ESearchCase::CaseSensitive)){
        FlagReadIfFound(frameNameSendMap, cleaned, TEXT("FRAMEID"));
    }else if(cleaned.StartsWith(TEXT("FRAMEIDCLOSE_"), ESearchCase::CaseSensitive)){
        FlagReadIfFound(frameNameCloseSendMap, cleaned, TEXT("FRAMEIDCLOSE"));
    }
}

void FCommunicationSharedFrameManager::FlagReadIfFound(std::map<FString, bool> &mapIn, const FString &message, const FString &prefix){
    FString frameName;
    if(ExtractFrameName(mapIn, message, prefix, frameName)){
        auto found = mapIn.find(frameName);
        found->second = true;
        DebugHelper::logMessage("FCommunicationSharedFrameManager:: Receive frame name MIRROR ok: ", frameName);
    }
}

bool FCommunicationSharedFrameManager::ExtractFrameName(const std::map<FString, bool> &mapIn, const FString &message, const FString &prefix, FString &outName){
    for(const auto &pair : mapIn){
        if(FSharedFrame *frame = FindFrame(pair.first)){
            if(message.Equals(frame->SharedFrameIdentifierMessage(prefix, pair.first), ESearchCase::CaseSensitive)){
                outName = pair.first;
                return true;
            }
        }
    }
    return false;
}

bool FCommunicationSharedFrameManager::IsSharedFrameIdentifierMessage(const FString &inName, FString &outData){
    //LogTemp: 
    //FCommunicationSharedFrameManager:: Check MIRROR: received payload  b'FRAMEID_/ueshmANNPathFinderSFNN_8_ANNPathFinderSFNN'  from numbytes 51

    
    const int32 start = inName.Find(TEXT("FRAMEID"), ESearchCase::CaseSensitive);
    if(start == INDEX_NONE){
        //DebugHelper::logMessage("FCommunicationSharedFrameManager:: Receive frame name MIRROR BAD: ", inName);
        return false;
    }

    int32 end = start;
    while(end < inName.Len()){
        const TCHAR character = inName[end];
        if(!FChar::IsAlnum(character) && character != TEXT('_') &&
           character != TEXT('/') && character != TEXT('-') && character != TEXT('.')){
            break;
        }
        ++end;
    }
    outData = inName.Mid(start, end - start);
    DebugHelper::logMessage("FCommunicationSharedFrameManager:: Receive frame name MIRROR OK?: ", outData);
    return true;
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

