#include "SharedMemoryActor.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

ASharedMemoryActor *ASharedMemoryActor::instance = nullptr;

ASharedMemoryActor *ASharedMemoryActor::GetInstance(UWorld *World){
    if(instance){
        return instance;
    }
    if(!World){
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = TEXT("ASharedMemoryActor");
    FVector Location = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;
    ASharedMemoryActor* Spawned = World->SpawnActor<ASharedMemoryActor>(
        ASharedMemoryActor::StaticClass(), Location, Rotation, SpawnParams
    );   
    instance = Spawned;
    return instance;
}




void ASharedMemoryActor::BeginPlay(){
    Super::BeginPlay();
    Open();
}

void ASharedMemoryActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    CleanFrame();
    instance = nullptr;
    Super::EndPlay(EndPlayReason);
}

void ASharedMemoryActor::Open(){
    if(Shared){
        return;
    }

    sharedFrameId = shm_open("/unreal_nn_shared", O_CREAT | O_RDWR, 0666);

    if (sharedFrameId < 0){
        // error
        return;
    }

    ftruncate(sharedFrameId, sizeof(FSharedFrame));

    Shared = (FSharedFrame*)mmap(
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
    }

}


void ASharedMemoryActor::WriteFrame(const unsigned char* Data)
{   
    if(!Shared){
        return;
    }

    if (Shared->Ready == 1){
        return;
    }
    std::memcpy(Shared->Data, Data, sizeof(Shared->Data));

    Shared->Ready = 1;
}


void ASharedMemoryActor::CleanFrame(){
    if(sharedFrameId >= 0){
        munmap(Shared, sizeof(FSharedFrame));
        close(sharedFrameId);
        shm_unlink("/unreal_nn_shared");
        Shared = nullptr;
    }
}



/*
python access


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