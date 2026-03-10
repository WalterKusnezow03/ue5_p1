#pragma once

#include "PlueckerCore/Math/Matrix3x3.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "PlueckerCore/Bone/JointConstraints/JointGroundedConstraint.h"

/*
Abstract Spatial transform matrix 6x6
*/
//base class for spatial transformations of spatial velocities and wrenches
class PLUECKERCORE_API SpatialTransform {


protected:
    //derived class specific
    virtual MMatrix Transform() = 0;
    virtual MMatrix TransformInverse() = 0;

    //axis for velocity / force propagation (use translation in bone.)
    virtual FVector &axis() = 0;

    //might update translation of bone if wanted,
    //might be extra constrained!
    virtual void OnForwardPlueckerFinished() = 0;

    //helper for matrices
    MMatrix Translation();
    MMatrix Rotation();

    MMatrix TranslationInverted();
    MMatrix RotationTransposed(); //R^T = R^-1

    

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

    //default implementation only for bones.
    virtual void forwardDeltaPluecker(
        FVector &angularVelocity, // w
        FVector &linearVelocity,  // v
        Matrix3x3 &outDeltaRotation,
        FVector &outDeltaTranslation,
        float deltatime
    );

    virtual void OnForwardPlueckerFinishedLogDeltaTranslation(
        const FVector & outDeltaTranslation
    ){};



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
    virtual void applyJointConstraint(FVector &w, FVector &v);
    virtual void applyGravityConstraint(FVector &w, FVector &v);

    FJointConstraint constraint;
    FJointGroundedConstraint groundContactConstraint;

    FCollisionQueryParams ignoreParams;






    //joint rotation
    Matrix3x3 RotationSO3;

    //bone direction by default (0,0,-length) for example
    FVector translation;

    //integrated location with forward pluecker v
    FVector resultTranslation;

    virtual void UpdateFloorContact(const MMatrix &prev, const MMatrix &current) = 0;
    bool IsGrounded(const MMatrix &prev);
    bool IsGrounded(FVector &Start);


    //world transformCache
    virtual void SafeWorldResultCache(const MMatrix &prevTransform, const MMatrix &endBone) = 0;


    //cache after build with martix mulitplication
    FVector worldLocationCache;
    FRotator worldRotatorCache;
    MMatrix worldTransformCache;



public:
    bool bIsGrounded(){
        return contactFloor;
    }


protected:
    //raycast
    bool contactFloor = false;
    FVector groundNormal;
    FVector groundTruth;
    float groundPenetration = 0.0f;
    FVector velocitycache;

    void SetGroundPenetration(bool result, const FVector &start, const FVector &hit);
    bool BelowGround(const FVector &check, const FVector &ground);
    bool BelowGround(const FVector &check, const FVector &ground, float epislon);
    void DrawGroundPenetration(const FVector &pos);

    //actor transform data extraction from cache, and interia world for root
public:
    FVector ActorTranslationFromCache() const;
    FRotator ActorRotationFromCache() const;
    FVector centerOfMassWorld(const FVector &comLocal) const;
    //actor transform data extraction from cache, and interia world for root
};