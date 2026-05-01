#pragma once

#include "CoreMinimal.h"

class SHAREDMEMORYPLUGIN_API FSharedFrame{

public:
    FSharedFrame();
    FSharedFrame(bool closeOnDestroyFlag);
    ~FSharedFrame();

    void Open(FString name, int bytes);
    void CleanFrame();

    //access unristricted, write immidiate, flagged as ready true, 1, if written
    void WriteData(const TArray<uint8> &bytes);
    void ReadData(TArray<uint8> &data);

    //returns: SharedFrameName_bytes
    FString SharedFrameIdentifier();
    virtual FString SharedFrameIdentifierMessage(FString prefix);
    virtual FString SharedFrameIdentifierMessage(FString prefix, FString postFixTag);

    bool SizeChanged(int sizeIn);

    bool TryReadReadyFlag();
    void MarkReadyFalse();

private:
    bool closeOnDestroy = false;

    //frame ptr
    uint8 *Shared = nullptr;

    FString pageName;
    int bytesAllocated = 0;
    int sharedFrameId = -1;

    int FlagReady(){
        return 0;
    }
    int FlagReadyFalse(){
        return 1;
    }
    int readyFlagSize();

    void MarkReady(bool flag);
    bool IsReady();

    int BytesWithoutReadyFlag();

    uint8 *pointerAfterReadyFlag();

    void MakeFrameName(FString name);



};

/*
----- PYTHON ACESS TO SHARED FRAME ------


//pythonCode.py

import mmap
import struct

SIZE = 1920 * 1080 * 3 + 16

shm = mmap.mmap(
    -1,
    SIZE,
    tagname="/unreal_nn_shared"
)



*/