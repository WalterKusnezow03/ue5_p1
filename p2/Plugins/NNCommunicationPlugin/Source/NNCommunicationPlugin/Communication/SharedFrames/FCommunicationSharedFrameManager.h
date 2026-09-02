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

protected:
    virtual void WriteData(FString pageName, const TArray<uint8> &data, bool &frameChanged) override;

    //new - testing needed! 
    void NotifyChangedFrames(
        APythonSocketBase *socket,
        std::map<FString, bool> &mapToMessage,
        FString prefixCommandForPython // might be "FRAMEID" for open, or "FRAMEIDCLOSE" for closing the shared memory
    );

    void NotifyChangedFrame(
        APythonSocketBase *socket,
        std::map<FString, bool> &mapToMessage,
        FString name, // shared frame name
        FString prefixCommandForPython
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
};
