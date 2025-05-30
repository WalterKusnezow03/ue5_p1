#pragma once

#include "Matrix3x3.h"
#include "CoreMath/Matrix/MMatrix.h"

/**
 * plücker 6x6 for forward for now
 */
class HUMANOID_API Matrix6x6 {

public:
    Matrix6x6();
    ~Matrix6x6();

    MMatrix operator*(MMatrix &prev);

    void forwardPluecker(
        FVector &angularVelocity, 
        FVector &linearVelocity,
        float deltatime
    );

    void setTranslation(FVector &other);

private:
    void applyConstraints(FVector &w, FVector &v);

    Matrix3x3 RotationSO3;
    FVector translation;

    //erstmal so
    FVector resultTranslation;
};