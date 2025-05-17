#pragma once
#include "CoreMinimal.h"
#include "p2/entities/customIk/MMatrix.h"
#include "p2/meshgen/customMeshActorBase.h"

#include "wheel.generated.h"

UCLASS()
class P2_API Awheel : public AcustomMeshActorBase {

GENERATED_BODY()

public:

    static Awheel *Construct(UWorld *world, float radiusIn);
    static Awheel *Construct(
        UWorld *world,
        float radiusIn,
        AActor *parent,
        FVector relativeLocation
    );

    void AttachToParent(AActor *parent, FVector &relativeLocation);

    Awheel();
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    FVector TickAngularAccelerationAndGetVelocity(float radPerSecond, float deltaTime);

    void processDirectionFromVehicleSpace(FVector &deltaDirection);

    FVector forwardDir();
    float thetaYawRad();




    FVector tractionForce();

    FVector corneringForce(float slipAngle);

    FVector frictionForceRoadDry(FVector &normal);
    FVector frictionForceRoadWet(FVector &normal);
    FVector frictionForce(FVector &normal, float haftReibungszahl);

    FVector AllForces(FVector &normal, float slipAngle);

protected:
    void generateMesh(float radiusIn);

    float angularVelocity = 0.0f;
    float radius = 50.0f;

    float yawAngle = 0.0f;

    MMatrix rotation;

    FVector moveToLocalRotationSpace(FVector &dir);
    FVector moveToWorldRotationSpace(FVector &localDir);
};