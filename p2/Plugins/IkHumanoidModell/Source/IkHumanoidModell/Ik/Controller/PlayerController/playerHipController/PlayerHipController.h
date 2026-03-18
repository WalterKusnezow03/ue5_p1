#pragma once

#include "IkHumanoidModell/Ik/Controller/HipController/HipController.h"

class IKHUMANOIDMODELL_API PlayerHipController : public HipController {

public:
    virtual void Tick(float deltatime) override;

    //not implemented: TODO
    void UpdateVelocity(float deltatime);

    MMatrix &getTranslationModfied();

protected:
    bool HasMoved();
    MMatrix modifiedTranslation;

    FVector prevTickLocation;

    virtual void applyLocomotion(float deltatime) override;
    virtual void updateInterpolatorLocomotion(float deltatime) override;
    void ApplyMaxHeightForForwardTarjectory(FVector &trajectory);
};