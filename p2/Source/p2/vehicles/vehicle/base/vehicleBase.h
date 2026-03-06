#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "GameCore/interfaces/Steeringinterface.h"
#include "CoreMath/animation/timer/Timer.h"
#include "vehicleBase.generated.h"

UCLASS()
class P2_API AvehicleBase : public AActor {

GENERATED_BODY()

public:
    AvehicleBase();
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    virtual void seatAsDriver(ISteeringinterface *driverIn);

protected:
    void processPlayerInputUnseatCheck(float deltaTime);

    void copyTransformOnStart();

    ISteeringinterface *driverInterface = nullptr;

    float velocity();

    float SlipAngle();

    MMatrix rotation;
    MMatrix translation;

    MMatrix transform();

    FVector moveDirectionToLocalRotationSpace(FVector &dir);
    FVector moveDirectionToWorldRotationSpace(FVector &dir);

    Timer unseatTimer;
    bool canUnseat();
    void TickUnseatTimer(float deltatime);
    void resetUnseatTimer();

    FVector velocityLocal;
    FVector velocityWorld;
};