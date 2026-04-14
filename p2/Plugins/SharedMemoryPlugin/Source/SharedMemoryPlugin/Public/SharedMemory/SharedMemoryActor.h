#pragma once

#include "CoreMinimal.h"
#include "SharedMemoryPlugin/Public/SharedMemory/FSharedFrame.h"

#include "SharedMemoryActor.generated.h"


UCLASS()
class SHAREDMEMORYPLUGIN_API ASharedMemoryActor : public AActor {
    GENERATED_BODY()
public:
    static ASharedMemoryActor *GetInstance(UWorld *world);

protected:
    static ASharedMemoryActor *instance;


    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void Open();
    void CleanFrame();
    void WriteFrame(const unsigned char *Data);

    FSharedFrame *Shared = nullptr;

    int sharedFrameId = -1;
};