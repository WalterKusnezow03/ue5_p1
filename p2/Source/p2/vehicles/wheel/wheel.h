#pragma once
#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

#include "wheel.generated.h"

UCLASS()
class P2_API Awheel : public AActor {

GENERATED_BODY()

public:
    bool debugPrint = false;

    static Awheel *Construct(UWorld *world, float radiusIn);
    static Awheel *Construct(
        UWorld *world,
        float radiusIn,
        AActor *parent,
        FVector relativeLocation
    );

    void disableTraction();
    void AttachToParent(AActor *parent, FVector &relativeLocation);

    Awheel();
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    FVector TickAngularAccelerationAndGetVelocity(float radPerSecond, float deltaTime);

    void processVelocityFromVehicleSpace(FVector &deltaDirection);

    FVector forwardDir();
    float thetaYawRad();
    void addYaw(float angleRad);
    void copyRotation(Awheel *ptr);

    FVector groundBelowWheel();

    FVector AllForces(FVector &normal, float slipAngle, float massPerWheel, FVector carLocalVelocity);

protected:
    FVector tractionForce();
    FVector corneringForce(float slipAngle);
    FVector normalForce(FVector &normal, float mass);

    FVector frictionForceRoadDry(FVector &normal, FVector &velocity, float mass);
    FVector frictionForceRoadWet(FVector &normal, FVector &velocity, float mass);
    FVector frictionForce(FVector &normal, float haftReibungszahl, FVector &velocity, float mass);

    float normalForceMagnitude(FVector &normal, float mass);

    float maxSteerAngleRad = 0.0f;
    bool tractionEnabled = true;
    float angularVelocity = 0.0f;
    float angularAcceleration = 0.0f;
    float radius = 50.0f;

    float yawAngle = 0.0f;

    MMatrix rotation;
    float clampRotationAdd(float yawAdd);

    FVector moveToLocalRotationSpace(FVector &dir);
    FVector moveToWorldRotationSpace(FVector &localDir);
};