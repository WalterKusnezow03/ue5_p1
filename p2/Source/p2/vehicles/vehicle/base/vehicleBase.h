#pragma once

#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/meshgen/customMeshActorBase.h"
#include "vehicleBase.generated.h"

UCLASS()
class P2_API AvehicleBase : public AcustomMeshActorBase {

GENERATED_BODY()

public:
    AvehicleBase();
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;


protected:
    float velocity();

    float SlipAngle();

    MMatrix rotation;
    MMatrix translation;

    MMatrix transform();

    FVector moveDirectionToLocalRotationSpace(FVector &dir);
    FVector moveDirectionToWorldRotationSpace(FVector &dir);

    

    FVector velocityLocal;
    FVector velocityWorld;
};