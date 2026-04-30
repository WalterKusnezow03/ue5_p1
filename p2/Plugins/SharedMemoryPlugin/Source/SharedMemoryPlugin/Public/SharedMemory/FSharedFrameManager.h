#pragma once

#include "SharedMemoryPlugin/Public/SharedMemory/FSharedFrame.h"
#include <map>


class SHAREDMEMORYPLUGIN_API FSharedFrameManager {

public:
    FSharedFrameManager();
    ~FSharedFrameManager();

    void ClearAllFrames();

    void Open(FString pageName, int bytes);
    bool HasFrame(FString pageName);


protected:


    virtual void WriteData(FString pageName, const TArray<uint8> &data, bool &frameChanged);

    FSharedFrame *FindFrame(FString name);
    std::map<FString, FSharedFrame> frames;
    TArray<FSharedFrame *> allFrames();

    bool NameValid(FString name);
    bool SizeValid(const TArray<uint8> &buffer);
};
