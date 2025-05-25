#pragma once

#include "CoreMinimal.h"
#include "GameCore/MeshGenBase/customMeshActorBase.h"
#include "GameCore/MeshGenBase/MeshData/aeroDynamic/AeroMeshData.h"
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
    UProceduralMeshComponent *MeshTail = nullptr;

    AeroMeshData meshDataMain;
    AeroMeshData meshDataUpDownBackWings;
    AeroMeshData meshDataTail;

    FVector linearVelocity;
    FVector angularVelocity;

    //std::vector<AeroMeshData *> allMeshDataHavingForce();
    std::map<AeroMeshData*, UProceduralMeshComponent*> allMeshDataHavingForce();

    float MassInKgTotal();
    FVector thrustForce();
    FVector thrustForceLocal();

    FVector transformVektorToLocalSpaceAndSpeed(FVector &dir);
    FVector transformVektorToLocalSpaceAndSpeed(
        UProceduralMeshComponent *component,
        FVector &dirWorldSpace
    );

    FVector transformVektorToWorldSpace(FVector &dirLocalSpace);
    FVector transformVektorToWorldSpace(
        UProceduralMeshComponent *component,
        FVector &dirLocalSpace
    );


    void drawForce(FVector &force, float deltatime);

    FMatrix actorRotationMatrix();
};