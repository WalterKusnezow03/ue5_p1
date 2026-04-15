#pragma once

#include "CoreMinimal.h"
#include "SharedMemoryPlugin/Public/SharedMemory/FSharedFrame.h"
#include <map>

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


    void Open(FString pageName, int bytes);
private:
    bool HasFrame(FString pageName);

    std::map<FString, FSharedFrame> frames;
};