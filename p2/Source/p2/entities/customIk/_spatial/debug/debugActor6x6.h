#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/_spatial/MMatrix6x6.h"
#include "p2/entities/customIk/_spatial/FVector6D.h"
#include "debugActor6x6.generated.h"

UCLASS()
class P2_API AdebugActor6x6 : public AActor {
    GENERATED_BODY()

public:
    static AdebugActor6x6 *Construct(UWorld *world);

    AdebugActor6x6();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
   // FVector6D initialJoint;

    void debugBuild(float DeltaTime);

    MMatrix6x6 a;
    MMatrix6x6 b;
    MMatrix6x6 c;
};