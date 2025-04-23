#pragma once

#include "CoreMinimal.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "p2/meshgen/MeshData/aeroDynamic/AeroMeshData.h"
#include <map>

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

    void processAeroForceAcceleration(FVector &forceOnMesh, float DeltaTime);
    void processTroqueAcceleration(FVector &torque, float deltaTime);

private:
    UProceduralMeshComponent *MeshBackWings = nullptr;

    AeroMeshData meshDataMain;
    AeroMeshData meshDataUpDownBackWings;

    FVector linearVelocity;
    FVector angularVelocity;

    //std::vector<AeroMeshData *> allMeshDataHavingForce();
    std::map<AeroMeshData*, UProceduralMeshComponent*> allMeshDataHavingForce();



    FVector transformVektorToLocalSpace(FVector &dir);
    FVector transformVektorToLocalSpace(
        UProceduralMeshComponent *component,
        FVector &dirWorldSpace
    );

    void drawForce(FVector &force, float deltatime);
};