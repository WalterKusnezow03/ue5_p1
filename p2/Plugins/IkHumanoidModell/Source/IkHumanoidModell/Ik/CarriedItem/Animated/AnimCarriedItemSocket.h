// Copyright Walter Kusnezow All Rights Reserved.

#pragma once

#include "IkHumanoidModell/Ik/CarriedItem/CarriedItemSocket.h"
#include "CoreMath/animation/FVectorInterpolator.h"
#include "CoreMath/animation/FRotatorInterpolator.h"

/// @brief isA Carried item but supports interpolating between local trajectories
class IKHUMANOIDMODELL_API AnimCarriedItemSocket : public CarriedItemSocket {
private:
    bool bLogMessage = false;
    bool bBlockRoation = true; //Debug

public:
    AnimCarriedItemSocket();
    virtual ~AnimCarriedItemSocket();

    virtual void Tick(
        float deltatime,
        MMatrix &translation,
        MMatrix &orientation //orientation of actor or even combined with limb or camera look direction.
    ) override;

    virtual void Tick(
        float deltatime,
        MMatrix &translation,
        MMatrix &orientationRaw, //orientation of skelleton
        FRotator &orientationCamera //orientation of camera
    ) override;


    ///@brief move to a location relative to the inner offset
    void TryMoveToLocal(FVector &pos, float timeFrame);

    ///@brief rotate relative to the inner socket rotation
    void TryRotateToLocal(FRotator &r, float timeFrame); 


    ///@brief move to a location relative to the inner offset, interrupt prev animation
    void MoveToLocal(FVector &pos, float timeFrame);

    ///@brief rotate relative to the inner socket rotation, interrupt prev animation
    void RotateToLocal(FRotator &r, float timeFrame); 

private:
    void TickAnimation(float DeltaTime);

    FVectorInterpolator localLocationInterpolator;
    FRotatorInterpolator localRotationInterpolator;

    FVector translationCopy;
    FRotator rotationCopy;
};