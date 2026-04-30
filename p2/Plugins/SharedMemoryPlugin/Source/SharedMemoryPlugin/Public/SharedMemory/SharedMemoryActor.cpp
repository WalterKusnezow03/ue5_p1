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
}

void ASharedMemoryActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    manager.ClearAllFrames();
    instance = nullptr;
    Super::EndPlay(EndPlayReason);
}


void ASharedMemoryActor::Open(FString name, int bytes){
    manager.Open(name, bytes);
}

bool ASharedMemoryActor::HasFrame(FString pageName){
    return manager.HasFrame(pageName);
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