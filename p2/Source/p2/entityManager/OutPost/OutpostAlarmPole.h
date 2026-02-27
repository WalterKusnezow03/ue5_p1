#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/customMeshActor.h"

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

    
    virtual void takedamage(FCustomHitResult &result) override;

    bool AlarmFunctionEnabled();

    void enableAlarmFunction();
    void disableAlarmFunction();

    void ResetParent(){
        outpostParentPointer = nullptr;
    }

protected:
    virtual bool isDestructable() override;
    void projectActorToGround();

private:
    void init();
    void createMesh();

    AOutpost *outpostParentPointer = nullptr;

    bool bAlarmFunctionEnabled = false;
};