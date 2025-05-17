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

    void pushGasPedal(float scalar);

protected:
    float gasPedalScalar = 0.0f;

    Awheel *frontRight = nullptr;
    Awheel *frontLeft = nullptr;
    Awheel *rearRight = nullptr;
    Awheel *rearLeft = nullptr;


    void createWheels();
    void createBaseCube();

    float radPerSecondAcceleration();
    void TickWheels(float deltatime);

    
    void applyDeltaRotation();
    void applyForces(float deltaTime);


    bool isNotZero(float value);

    void IntegrateMovement(FVector &accelerationlocal, float deltatime);
};