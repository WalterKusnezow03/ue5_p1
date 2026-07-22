#pragma once
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FAbstractPropertyBase.h"

class IKHUMANOIDMODELL_API FLocomotionProperty : public FAbstractPropertyBase{

public:
    void SetMaxVelocity(float inVelocity){
        maxVelocity = Validate(inVelocity);
    }

    void SetMinVelocity(float inVelocity){
        minVelocity = Validate(inVelocity);
    }

    float GetMaxVelocity(){
        return maxVelocity;
    }

    float GetMinVelocity(){
        return minVelocity;
    }

    void ClampHorizontal(FVector &velocity){
        FVector2D velocity2D(velocity.X, velocity.Y);
        ClampHorizontal(velocity2D);
        velocity.X = velocity2D.X;
        velocity.Y = velocity2D.Y;
    }

    void ClampHorizontal(FVector2D &velocity2D){

        if(velocity2D.Size() > maxVelocity){
            velocity2D = velocity2D.GetSafeNormal() * maxVelocity;
        }
        if(velocity2D.Size() < minVelocity){
            velocity2D = velocity2D.GetSafeNormal() * minVelocity;
        }
    }

protected:
    float maxVelocity = 200.0f;
    float minVelocity = 100.0f;
};