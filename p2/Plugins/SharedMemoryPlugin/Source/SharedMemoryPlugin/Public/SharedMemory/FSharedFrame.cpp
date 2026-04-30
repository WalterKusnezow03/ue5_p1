#include "FSharedFrame.h"

#include "DebugPlugin/DebugHelper.h"
#include <errno.h>
#include <string.h>

FSharedFrame::FSharedFrame(){
    closeOnDestroy = true;
}

FSharedFrame::FSharedFrame(bool closeOnDestroyFlag){
    closeOnDestroy = closeOnDestroyFlag;
}

FSharedFrame::~FSharedFrame(){
    if(closeOnDestroy){ //only if allowed: can be disallowed in constructor!
        //CleanFrame(); //Cant happen here! - manual call needed if copy constructor called!
    }
    
}

FString FSharedFrame::SharedFrameIdentifier(){
    return FString::Printf(TEXT("%s_%d"), *pageName, bytesAllocated);
}

FString FSharedFrame::SharedFrameIdentifierMessage(FString prefix, FString postFixTag){
    return FString::Printf(TEXT("%s_%s"), *SharedFrameIdentifierMessage(prefix), *postFixTag);
}

FString FSharedFrame::SharedFrameIdentifierMessage(FString prefix){
    return FString::Printf(TEXT("%s_%s"), *prefix, *SharedFrameIdentifier());
}

bool FSharedFrame::SizeChanged(int sizeIn){
    return (sizeIn + readyFlagSize()) != bytesAllocated;
}

void FSharedFrame::Open(FString name, int bytes){
    if(Shared){
        return;
    }
    if(name.Len() < 0){
        return;
    }
    if(bytes <= 0){
        return;
    }

    DebugHelper::logMessage("FSharedFrame::Open Try", bytes);

    //copy bytes allocated for later free page
    bytesAllocated = bytes + readyFlagSize(); // add ready flag at front

    MakeFrameName(name);
    DebugHelper::logMessage("FSharedFrame::Open Try: ", pageName);
    sharedFrameId = shm_open(TCHAR_TO_ANSI(*pageName), O_CREAT | O_RDWR, 0666);
    //sharedFrameId = shm_open("/unreal_nn_shared", O_CREAT | O_RDWR, 0666);

    //fixed.
    if (sharedFrameId < 0){
        // error
        
        DebugHelper::logMessage("FSharedFrame::Open shm_open failed errno", errno);
        DebugHelper::logMessage("FSharedFrame::Open shm_open failed reason", strerror(errno));
        DebugHelper::logMessage("FSharedFrame::Open ID Failed", sharedFrameId);
        Shared = nullptr;
        return;
    }

    ftruncate(sharedFrameId, bytesAllocated);

    //POSIX Api
    Shared = (uint8*)mmap(
        nullptr,
        bytesAllocated, //sizeof(FSharedFrame),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        sharedFrameId,
        0
    );

    if (Shared == MAP_FAILED)
    {
        DebugHelper::logMessage("FSharedFrame::Open FAILED");
        // error
        Shared = nullptr; //(?)
        sharedFrameId = -1;
    }else{
        DebugHelper::logMessage("FSharedFrame::Open SUCESS", bytes);
    }
}

void FSharedFrame::MakeFrameName(FString name){
    const int MAX_SHM_NAME = 30; // sicher unter Limit bleiben!! //DO NOT REMOVE
    pageName = FString::Printf(TEXT("/ueshm%s"), *name);
    if (pageName.Len() > MAX_SHM_NAME)
    {
        DebugHelper::logMessage("Shared memory name too long, truncating");

        pageName = pageName.Left(MAX_SHM_NAME);
    }
}





void FSharedFrame::WriteData(const TArray<uint8> &bytes)
{   
    if(!Shared){
        return;
    }

    if(bytes.Num() <= 0){
        return;
    }

    const uint8 *ptr = bytes.GetData();
    uint8 *Dest = pointerAfterReadyFlag();

    /*
    FMemory::Memcpy( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    MarkReady(false);
    int copy = bytesAllocated - readyFlagSize();
    FMemory::Memcpy(Dest, ptr, copy * sizeof(uint8)); //copy without offset size
    MarkReady(true);

    DebugHelper::logMessage("FSharedFrame::WriteData ", copy);
}

uint8 *FSharedFrame::pointerAfterReadyFlag(){
    if(Shared){
        return Shared + readyFlagSize(); // offset to after ready flag flag!
    }
    return nullptr;
}

int FSharedFrame::readyFlagSize(){
    return sizeof(int);
}


void FSharedFrame::MarkReady(bool ready){
    if(!Shared){
        return;
    }

    //dest, src, size T
    int flag = ready ? 1 : 0;
    FMemory::Memcpy(Shared, &flag, sizeof(int)); // copy casted data
    readyStatus = ready;
}

void FSharedFrame::CleanFrame(){
    if(!Shared){
        return;
    }

    if(sharedFrameId >= 0){
        munmap(Shared, bytesAllocated);
        // munmap(Shared, sizeof(FSharedFrame));
        close(sharedFrameId);
        shm_unlink(TCHAR_TO_ANSI(*pageName));
        // shm_unlink("/unreal_nn_shared");
        Shared = nullptr;
    }
}
