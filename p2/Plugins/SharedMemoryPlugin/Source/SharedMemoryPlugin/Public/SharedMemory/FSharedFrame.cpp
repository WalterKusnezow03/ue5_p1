#include "FSharedFrame.h"

FSharedFrame::FSharedFrame(){

}

FSharedFrame::~FSharedFrame(){
    CleanFrame();
}

FString FSharedFrame::SharedFrameIdentifier(){
    return FString::Printf(TEXT("%s-%d"), *pageName, bytesAllocated);
}

FString FSharedFrame::SharedFrameIdentifierMessage(FString prefix){
    return FString::Printf(TEXT("%s-%s"), *prefix, *SharedFrameIdentifier());
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

    //copy bytes allocated for later free page
    bytesAllocated = bytes + readyFlagSize(); // add ready flag at front

    pageName = FString::Printf(TEXT("/unreal_shared_page_%s"), *name);
    sharedFrameId = shm_open(TCHAR_TO_ANSI(*pageName), O_CREAT | O_RDWR, 0666);
    //sharedFrameId = shm_open("/unreal_nn_shared", O_CREAT | O_RDWR, 0666);

    if (sharedFrameId < 0){
        // error
        Shared = nullptr;
        return;
    }

    ftruncate(sharedFrameId, bytes);

    Shared = (uint8*)mmap(
        nullptr,
        sizeof(FSharedFrame),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        sharedFrameId,
        0
    );

    if (Shared == MAP_FAILED)
    {
        // error
        Shared = nullptr; //(?)
        sharedFrameId = -1;
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
    FMemory::Memcpy(Dest, ptr, sizeof(bytesAllocated)); //copy without offset size
    MarkReady(true);
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
