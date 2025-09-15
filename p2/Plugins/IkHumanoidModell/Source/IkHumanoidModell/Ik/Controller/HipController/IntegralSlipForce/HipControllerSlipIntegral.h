#pragma once

#include "IkHumanoidModell/Ik/Controller/HipController/HipController.h"

/// @brief derived class from hip controller: Sets up integral slip force instead of
/// using static slip force by state (see backward phase creating forward motion - integral and lift off frames
/// used instead: unstable, pushed into this class to remove issues)
class IKHUMANOIDMODELL_API HipControllerSlipIntegral : public HipController {

public:
    HipControllerSlipIntegral();
    virtual ~HipControllerSlipIntegral() override;

    

    

protected:

    virtual void setupBackwardInterpolation(float dynamicMotionTime) override;
    virtual void applySlipForce(float deltatime) override;
    void applyStancePhaseSLIPForce(float deltatime);
};