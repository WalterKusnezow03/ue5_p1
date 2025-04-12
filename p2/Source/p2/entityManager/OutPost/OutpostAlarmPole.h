#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActor.h"

#include "OutpostAlarmPole.generated.h"

class AOutpost;

UCLASS()
class P2_API AOutpostAlarmPole : public AcustomMeshActor{
    GENERATED_BODY()


public:
    static AOutpostAlarmPole *Construct(UWorld *world, FVector &location);

    AOutpostAlarmPole();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    using AcustomMeshActor::takedamage;
    virtual void takedamage(int d, FVector &hitpoint, bool surpressed) override;

    bool AlarmFunctionEnabled();

    void enableAlarmFunction();
    void disableAlarmFunction();

protected:
    virtual bool isDestructable() override;
    void projectActorToGround();

private:
    void init();
    void createMesh();

    AOutpost *outpostParentPointer = nullptr;

    bool bAlarmFunctionEnabled = false;
};