#pragma once
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FAbstractPropertyBase.h"

class IKHUMANOIDMODELL_API FLocomotionProperty : public FAbstractPropertyBase{

public:
    void SetMaxVelocity(float inVelocity){
        maxVelocity = Validate(inVelocity);
    }

    float GetMaxVelocity(){
        return maxVelocity;
    }

protected:
    float maxVelocity = 200.0f;
};