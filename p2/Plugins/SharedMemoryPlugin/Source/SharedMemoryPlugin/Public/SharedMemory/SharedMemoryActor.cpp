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
    frames.clear();
    instance = nullptr;
    Super::EndPlay(EndPlayReason);
}


void ASharedMemoryActor::Open(FString name, int bytes){
    if(bytes > 0 && name.Len() > 0){
        if(!HasFrame(name)){
            frames[name] = FSharedFrame();
            FSharedFrame &ref = frames[name];
            ref.Open(name, bytes);
        }
    }
}   

bool ASharedMemoryActor::HasFrame(FString pageName){
    return frames.find(pageName) != frames.end();
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