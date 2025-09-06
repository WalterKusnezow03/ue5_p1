#pragma once 

#include "CoreMinimal.h"
#include "GcGameCore/Collection/Collector.h"

#include "GcLauncher.generated.h"

UCLASS()
class GCGAMECORE_API AGcLauncher : public AActor {
    GENERATED_BODY()

private:
    static AGcLauncher *instancePtr;

public:
    Collector collection;

    static AGcLauncher *MakeInstance(UWorld *world);

    static AGcLauncher *Instance();

protected:
    // EndPlay override
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;




};
