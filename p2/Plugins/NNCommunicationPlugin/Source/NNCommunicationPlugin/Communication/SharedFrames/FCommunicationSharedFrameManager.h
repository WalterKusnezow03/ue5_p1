#pragma once

#include "SharedMemoryPlugin/Public/SharedMemory/FSharedFrameManager.h"

class APythonSocketBase;

/// @brief adds flags for frame name shared to python
class NNCOMMUNICATIONPLUGIN_API FCommunicationSharedFrameManager : public FSharedFrameManager {

public:
    void WriteData(FString name, const TArray<uint8> &data);
    void ReadData(
        FString pageName,
        TArray<uint8> &data,
        int bytesToRead
    );
    void MakeSureExists(FString pageName, int bytes);

    //call on tick if socket connected
    void NotifyChangedFrames(APythonSocketBase *socket);

    void ClosePage(FString pageName);

    //notify python receival of the send data
    //if not approved it will be kept send.
    //verification ensures data has been received by python.
    void NotifyMessageMirror(FString message);

protected:
    virtual void WriteData(FString pageName, const TArray<uint8> &data, bool &frameChanged) override;

    //new - testing needed! 
    void NotifyChangedFrames(
        APythonSocketBase *socket,
        std::map<FString, bool> &mapToMessage,
        FString prefixCommandForPython,// might be "FRAMEID" for open, or "FRAMEIDCLOSE" for closing the shared memory
        bool waitForMirror 
    );

    void NotifyChangedFrame(
        APythonSocketBase *socket,
        std::map<FString, bool> &mapToMessage,
        FString name, // shared frame name
        FString prefixCommandForPython,
        bool waitForMirror 
    );

    //deprecated! 
    /*void NotifyChangedFrame(
        FString name,
        APythonSocketBase *socket
    );*/

    //notify open
    std::map<FString, bool> frameNameSendMap;

    //notify close
    std::map<FString, bool> frameNameCloseSendMap;

    void FlagReadIfFound(std::map<FString, bool> &mapIn, const FString &message, const FString &prefix);
    bool ExtractFrameName(const std::map<FString, bool> &mapIn, const FString &message, const FString &prefix, FString &outName);
    static bool IsSharedFrameIdentifierMessage(const FString &inName, FString &outData);
};
