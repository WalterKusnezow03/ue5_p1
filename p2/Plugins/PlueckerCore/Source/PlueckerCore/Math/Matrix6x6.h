#pragma once

#include "Matrix3x3.h"
#include "CoreMath/Matrix/MMatrix.h"

/**
 * plücker 6x6 for forward for now
 */
class PLUECKERCORE_API Matrix6x6 {

public:
    Matrix6x6();
    ~Matrix6x6();

    MMatrix operator*(MMatrix &prev);

    void forwardPluecker(
        FVector &angularVelocity, 
        FVector &linearVelocity,
        float deltatime
    );

    void forwardDeltaPluecker(
        FVector &angularVelocity, // w
        FVector &linearVelocity,  // v
        Matrix3x3 &outDeltaRotation,
        FVector &outDeltaTranslation,
        float deltatime
    );

    void setTranslation(FVector &other);

    FVector getTranslation()const{
        return translation;
    }

private:
    //v will be right not eliminated by default!
    void applyConstraints(FVector &w, FVector &v);

    //joint rotation
    Matrix3x3 RotationSO3;

    //bone direction by default (0,0,-length) for example
    FVector translation;

    //integrated location
    FVector resultTranslation;
};