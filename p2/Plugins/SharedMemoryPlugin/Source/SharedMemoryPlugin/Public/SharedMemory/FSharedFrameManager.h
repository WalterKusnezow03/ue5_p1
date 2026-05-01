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

    //doesnt create frame if not found! -> buffer will be cleared!
    void TryReadDataTo(FString pageName, TArray<uint8> &buffer);


    bool TryReadReadyFlag(FString pageName);
    void MarkReadyFalse(FString pageName);

protected:


    virtual void WriteData(FString pageName, const TArray<uint8> &data, bool &frameChanged);
    void ReadData(
        FString pageName, 
        TArray<uint8> &data, 
        int bytesToRead, 
        bool &frameChanged
    );
    void MakeSureExists(FString pageName, int bytes, bool &frameChanged);

    
    

    FSharedFrame *FindFrame(FString name);
    std::map<FString, FSharedFrame> frames;
    TArray<FSharedFrame *> allFrames();

    bool NameValid(FString name);
    bool SizeValid(const TArray<uint8> &buffer);
};
