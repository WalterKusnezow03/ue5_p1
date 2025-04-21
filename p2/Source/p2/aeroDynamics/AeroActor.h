#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "p2/meshgen/MeshData/aeroDynamic/AeroMeshData.h"

#include "AeroActor.generated.h"

UCLASS()
class AAeroActor : public AcustomMeshActorBase {
    GENERATED_BODY()

public:
    AAeroActor();

    static AAeroActor *Construct(UWorld *world, FVector &location);

    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

protected:
    virtual void initMesh();
    UMaterialInterface *wingMaterialPointer();

    void processTroqueAcceleration(FVector &torque, float deltaTime);

private:
    AeroMeshData meshDataMain;

    FVector linearVelocity;
    FVector angularVelocity;

    FVector transformVektorToLocalSpace(FVector &dir);

    void drawForce(FVector &force, float deltatime);
};