#pragma once

#include "Matrix3x3.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "PlueckerCore/Bone/JointConstraints/JointGroundedConstraint.h"

/**
 * plücker 6x6 for forward for now
 */
class PLUECKERCORE_API Matrix6x6 {

private:
    UWorld *world = nullptr;

public:
    bool bLogMessage = false;

    Matrix6x6();
    ~Matrix6x6();

    MMatrix operator*(const MMatrix &prev);

    Matrix6x6(const Matrix6x6 &other);
    Matrix6x6 &operator=(const Matrix6x6 &other);

    void SetWorld(UWorld *worldIn);

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
    void OverrideRotation(const FRotator &r);

    Matrix3x3 GetRotation()const;
    void CopyRotationTo(MMatrix &copyInside)const;

    // --- external override of transform ---

    void backwardWrench(
        FVector &moment, //n
        FVector &force,  //f
        float deltatime
    );
    void backwardDeltaWrench(
        FVector &moment, // n
        FVector &force,  // f
        float deltatime
    );

    void UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn);

    FVector Force(float mass);
    FVector Torque(const FVector &force, const FVector &centerOfMass);

private:
    FVector Torque(const Matrix3x3 &rotationSpace, const FVector &force, const FVector &centerOfMass);
    FVector NormalForce(float mass);
    FVector GravityForce(float mass);

    void UpdateGroundConstraint(const MMatrix &worldResult);
    void UpdateGroundConstraintPitchAndPosition();
    void UpdateGroundConstraintRoll(const MMatrix &worldResult);

    void ShowPosition(const MMatrix &other);
    void ShowPosition(FVector pos);
    void ShowVector(const FVector &vec, float size, FColor color);

    //v will be right not eliminated by default!
    void applyConstraints(FVector &w, FVector &v);
    FJointConstraint constraint;
    FJointGroundedConstraint groundContactConstraint;

    FCollisionQueryParams ignoreParams;

    //joint rotation
    Matrix3x3 RotationSO3;

    //bone direction by default (0,0,-length) for example
    FVector translation;

    //integrated location with forward pluecker v
    FVector resultTranslation;

    MMatrix LocalTransform();
    void UpdateFloorContact(const MMatrix &prev, const MMatrix &current);
    bool IsGrounded(const MMatrix &prev);
    bool IsGrounded(FVector &Start);


    //world transformCache
    void SafeWorldResultCache(const MMatrix &other);
    FVector worldLocationCache;

    //raycast
    bool contactFloor = false;
    FVector groundNormal;
    float groundPenetration = 0.0f;

    void SetGroundPenetration(bool result, const FVector &start, const FVector &hit);
    bool BelowGround(const FVector &check, const FVector &ground);
    void DrawGroundPenetration(const FVector &pos);

    
};