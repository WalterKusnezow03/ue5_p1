#pragma once

#include "SharedMemoryPlugin/Public/SharedMemory/FSharedFrameManager.h"

class APythonSocketBase;

/// @brief adds flags for frame name shared to python
class NNCOMMUNICATIONPLUGIN_API FCommunicationSharedFrameManager : public FSharedFrameManager {

public:
    void WriteData(FString name, const TArray<uint8> &data);

    //call on tick if socket connected
    void NotifyChangedFrames(APythonSocketBase *socket);

protected:
    virtual void WriteData(FString pageName, const TArray<uint8> &data, bool &frameChanged) override;
    void NotifyChangedFrame(
        FString name,
        APythonSocketBase *socket
    );

    std::map<FString, bool> frameNameSendMap;
};
