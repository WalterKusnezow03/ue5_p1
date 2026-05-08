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
    if(bUseMutex){
        return FString::Printf(
            TEXT("%s_%d_%s"), 
            *pageName, 
            bytesAllocated,
            *semaphoreMutexName
        );
    }
    return FString::Printf(
        TEXT("%s_%d"), 
        *pageName, 
        bytesAllocated
    );
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
    MakeSemaphoreName(name);
    DebugHelper::logMessage("FSharedFrame::Open Try: ", pageName);

    //std::string base = TCHAR_TO_ANSI(*pageName);
    sharedFrameId = shm_open(TCHAR_TO_ANSI(*pageName), O_CREAT | O_RDWR, 0666);
    //shm_open(TCHAR_TO_ANSI(*pageName), O_CREAT | O_RDWR, 0666);
   
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
        semaphoreMutex = nullptr;
        sharedFrameId = -1;
    }else{
        DebugHelper::logMessage("FSharedFrame::Open SUCESS", bytes);


        InitSemaphore();
    }
}

void FSharedFrame::InitSemaphore(){
    if(bUseMutex){
        semaphoreMutex = sem_open(TCHAR_TO_ANSI(*semaphoreMutexName), O_CREAT, 0666, 1);
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

void FSharedFrame::MakeSemaphoreName(FString name){
    const int MAX_NAME = 30; // sicher unter Limit bleiben!! //DO NOT REMOVE

    semaphoreMutexName = FString::Printf(TEXT("/semW%s"), *name);
    if (semaphoreMutexName.Len() > MAX_NAME){
        semaphoreMutexName = semaphoreMutexName.Left(MAX_NAME);
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

    int copy = BytesWithoutReadyFlag();
    if(bytes.Num() != copy){
        DebugHelper::logMessage("FSharedFrame::WriteData Cant Write Data", bytes.Num());
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
    Down();
    MarkReady(false);
    FMemory::Memcpy(Dest, ptr, copy * sizeof(uint8)); // copy without offset size
    MarkReady(true);
    Up();

    DebugHelper::logMessage("FSharedFrame::WriteData ", copy);
}


void FSharedFrame::Down(){
    if(semaphoreMutex && bUseMutex){
        sem_wait(semaphoreMutex);
    }
}


void FSharedFrame::Up(){
    if(semaphoreMutex && bUseMutex){
        sem_post(semaphoreMutex);
    }
}





void FSharedFrame::ReadData(TArray<uint8> &data){
    data.Empty();
    if(!Shared){
        return;
    }

    int copy = BytesWithoutReadyFlag();
    data.SetNumUninitialized(copy);

    uint8 *ptr = pointerAfterReadyFlag();
    void *Dest = data.GetData();

    Down();
    FMemory::Memcpy(Dest, ptr, copy * sizeof(uint8)); //copy without offset size
    Up();

    ptr = nullptr;
    Dest = nullptr;
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

int FSharedFrame::BytesWithoutReadyFlag(){
    return bytesAllocated - readyFlagSize();
}

void FSharedFrame::MarkReady(bool ready){
    if(!Shared){
        return;
    }

    //dest, src, size T
    int flag = ready ? 1 : 0;
    FMemory::Memcpy(Shared, &flag, sizeof(int)); // copy casted data
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

    // --- SEMAPHORE CLEANUP ---

    if (semaphoreMutex){
        sem_close(semaphoreMutex);
        sem_unlink(TCHAR_TO_ANSI(*semaphoreMutexName));
        semaphoreMutex = nullptr;
    }
    
}



bool FSharedFrame::TryReadReadyFlag(){
    if(!Shared){
        return false;
    }

    int flag = 0;
    // FMemory::Memcpy(dest, src, sizeof(T))
    Down();
    FMemory::Memcpy(&flag, Shared, sizeof(int)); // copy casted data
    Up();
    return flag == 1; //ready.
}

void FSharedFrame::MarkReadyFalse(){
    Down();
    MarkReady(false);
    Up();
}

//void MarkReady(bool flag);