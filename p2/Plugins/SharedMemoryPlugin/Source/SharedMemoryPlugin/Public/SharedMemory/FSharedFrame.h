#pragma once

#include "CoreMinimal.h"

class SHAREDMEMORYPLUGIN_API FSharedFrame{

public:
    FSharedFrame();
    ~FSharedFrame();

    void Open(FString name, int bytes);
    void CleanFrame();
    void WriteData(const TArray<uint8> &bytes);

    //returns: SharedFrameName_bytes
    FString SharedFrameIdentifier();
    FString SharedFrameIdentifierMessage(FString prefix);

private:
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

    bool readyStatus = true;
    uint8 *pointerAfterReadyFlag();
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