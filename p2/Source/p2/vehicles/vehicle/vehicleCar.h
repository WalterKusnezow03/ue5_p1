#pragma once

#include "CoreMinimal.h"
#include "p2/vehicles/vehicle/base/vehicleBase.h"
#include "p2/vehicles/wheel/wheel.h"

#include "vehicleCar.generated.h"

UCLASS()
class P2_API AvehicleCar : public AvehicleBase {

GENERATED_BODY()

public:
    static AvehicleCar *Construct(UWorld *world);

    AvehicleCar();
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    bool showScreenLog = false;

protected:
    
    void processDriverInput(float deltatime);
    void pushGasPedal(float scalar);
    float brakeScalar();
    void steer(float deltatime, float scalarDirection);

    float gasPedalScalar = 0.0f;

    Awheel *frontRight = nullptr;
    Awheel *frontLeft = nullptr;
    Awheel *rearRight = nullptr;
    Awheel *rearLeft = nullptr;

    float steerSpeedRadPerSecond = M_PI * 3.0f; 
    float rearFrontDistance = 500.0f;
    float mass = 2000.0f; //in kg

    void findWheels();
    void findRearFrontDistance();

    void createMeshIfNeeded();
    void createWheels();
    void createBaseCube();


    float radPerSecondAcceleration();
    void TickWheels(float deltatime);

    
    void applyDeltaRotation(float deltaTime);
    void applyForces(float deltaTime);


    bool isNotZero(float value);

    void IntegrateMovement(FVector &accelerationlocal, float deltatime);

    
};