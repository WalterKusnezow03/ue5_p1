#pragma once

#include "PlueckerCore/Math/Matrix3x3.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "PlueckerCore/Bone/JointConstraints/JointGroundedConstraint.h"

/*
Spatial transform matrix 6x6
*/
//base class for spatial transformations of spatial velocities and wrenches
class PLUECKERCORE_API SpatialTransform {


protected:
    //derived class specific
    virtual MMatrix Transform() = 0;

    //axis for velocity / force propagation (use translation in bone.)
    virtual FVector &axis() = 0;

    //helper for matrices
    MMatrix Translation();
    MMatrix Rotation();


    

protected:
    UWorld *world = nullptr;

    int staticGroundHeight = 0;

public:
    bool bLogMessage = false;

    void SetWorld(UWorld *worldIn);

    SpatialTransform();
    ~SpatialTransform();
    MMatrix operator*(const MMatrix &prev);
    SpatialTransform(const SpatialTransform &other);
    SpatialTransform &operator=(const SpatialTransform &other);

    

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
        FVector &force //f
    );

    void UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn);

    FVector Force(float mass);
    virtual FVector Torque(const FVector &force, const FVector &centerOfMass);



protected:
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

    
    void UpdateFloorContact(const MMatrix &prev, const MMatrix &current);
    bool IsGrounded(const MMatrix &prev);
    bool IsGrounded(FVector &Start);


    //world transformCache
    void SafeWorldResultCache(const MMatrix &other);
    FVector worldLocationCache;



public:
    bool bIsGrounded(){
        return contactFloor;
    }


protected:
    //raycast
    bool contactFloor = false;
    FVector groundNormal;
    float groundPenetration = 0.0f;
    FVector velocitycache;

    void SetGroundPenetration(bool result, const FVector &start, const FVector &hit);
    bool BelowGround(const FVector &check, const FVector &ground);
    void DrawGroundPenetration(const FVector &pos);

    
};