#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

/// @brief struct to organize angular and linear velocity for joint propagation
struct PLUECKERCORE_API FJointKinematicPropagatePackage {

    float deltatime = 0.0f; 
    FVector w; // angular velocity
    FVector v; // linear velocity
    MMatrix transform;

    void Setup(float deltatimeIn, FVector &wIn, FVector &vIn, MMatrix &transformIn){
        deltatime = deltatimeIn;
        w = wIn;
        v = vIn;
        transform = transformIn;
    }
};