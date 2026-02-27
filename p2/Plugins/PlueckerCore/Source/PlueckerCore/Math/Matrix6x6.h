#pragma once

#include "Matrix3x3.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"

/**
 * plücker 6x6 for forward for now
 */
class PLUECKERCORE_API Matrix6x6 {

public:
    Matrix6x6();
    ~Matrix6x6();

    MMatrix operator*(const MMatrix &prev);

    Matrix6x6(const Matrix6x6 &other);
    Matrix6x6 &operator=(const Matrix6x6 &other);

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

    void OverrideConstraint(FJointConstraint &in);
    FJointConstraint &GetConstraint();

    // --- external override of transform ---
    //override rotation of joint (needed for Ik Humanoid modell)
    void OverrideRotation(const MMatrix &rotationMatIn);
    void OverrideRotation(const Matrix3x3 &rotationMatIn);

    Matrix3x3 GetRotation()const;

    // --- external override of transform ---

private:
    //v will be right not eliminated by default!
    void applyConstraints(FVector &w, FVector &v);
    FJointConstraint constraint;

    //joint rotation
    Matrix3x3 RotationSO3;

    //bone direction by default (0,0,-length) for example
    FVector translation;

    //integrated location
    FVector resultTranslation;
};