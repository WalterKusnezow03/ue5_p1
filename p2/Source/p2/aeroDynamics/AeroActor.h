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

    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

protected:
    virtual void initMesh();
    UMaterialInterface *wingMaterialPointer();

private:
    AeroMeshData meshDataMain;
    FVector velocity;

    FVector transformVektorToLocalSpace(FVector &dir);
    
};