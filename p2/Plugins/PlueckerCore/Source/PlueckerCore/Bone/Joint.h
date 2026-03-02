#pragma once


#include "CoreMinimal.h"
#include "PlueckerCore/Math/Matrix6x6.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Math/SpatialVector.h"
#include "PlueckerCore/Interface/FJointKinematicPropagatePackage.h"

class PLUECKERCORE_API Joint {

public:
    
    Joint();
    Joint(FVector translationVector);
    Joint(FVector translationVector, UWorld *worldIn);
    ~Joint();

    Joint(const Joint &other);
    Joint &operator=(const Joint &other);

    // add force
    //mass internal property of joint
protected:
    void AddTorque(const FVector &torque, float deltaTime);
public:
    void AddForce(const FVector &force, float deltaTime); //mass internal property of joint

    // ---- external building of joints ----

    FJointKinematicPropagatePackage GeneratePackage(MMatrix &transform, float deltaTime);

    //external build, external propagation ---> INCLUDES GRAVITY FORCE AND TORQUE
    //returns updated transform
    MMatrix TickAndBuildThisJoint(FJointKinematicPropagatePackage &package);
    MMatrix TickAndBuildThisJoint(
        float deltaTime,
        FVector &w,
        FVector &v,
        const MMatrix &inTransform
    );
    MMatrix TickAndBuildThisJoint(
        float deltaTime,
        const MMatrix &inTransform
    );
    // external build, external propagation

    // external transform updates
    void OverrideJointRotation(const MMatrix &rotationMatrix);
    void OverrideJointRotationTransposed(const Joint &other);
    void OverrideWorldLocation(FVector pos);

    // ---- external building of joints ----




    //internal build to child
    void AddChild(Joint &childIn);
    void TickAndBuildRecursive(
        float deltatime,
        FVector &w,
        FVector &v,
        MMatrix &transform
    );
    //internal build to child

    void SetWorld(UWorld *worldIn);

    void DrawJointLocation(float deltatime);

    void SetDrawColor(FColor colorIn);
    void SetDrawColorRecursive(FColor colorA, FColor colorB, int i);

    void SetBoneTranslationDirection(FVector direction);
    FVector BoneTranslationDirection()const ;


    void OverrideConstraint(FJointConstraint &in);
    FJointConstraint &GetConstraint();

    void SetInteriaMatrixAuto();
    void SetInteriaMatrix(const Matrix3x3 &interiaIn);

private:
    FColor color = FColor::Red;
    float distanceToGroundedFlag = 10.0f;

    MMatrix transformCopy;
    Matrix6x6 spatialTransform;

    //saves the current w and v of this joint, allows to add torque and force.
    SpatialVector spatialVelocity;

    //for force accumulation
    Matrix3x3 interiaInverse;
    float mass = 1000.0f; //10000.0f
    FVector centerOfMass = FVector(0.5, 0.5, 0.5); //example value

   

    
    FVector GravityForce();
    FVector Torque(FVector force);

    UWorld *world = nullptr;
    void draw(MMatrix &a, MMatrix &b, float deltatime);
    void LogSpatialVelocities(FString Prefix, const FVector &w, const FVector &v);

    TArray<Joint> children;

    void UpdateSpatialVelocityAndPassedVelocities(FVector &w, FVector &v);

    // ---- GRAVITY EXPERIMENTAL ----
public:
    void TickGravity6DOF(float deltaTime, MMatrix &updateTransform);
    void TickGravity(float deltatime, MMatrix &updateTransform); //no constraint unlock
    void TickGravityAndAddUpdateToSptialVector(float deltaTime);

    void TickUpdateGroundedFlag();
    bool JointIsGrounded();
    
    void UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn){
        ignoreParams = ignoreParamsIn;
    }

protected:
    //new experimental
    bool bIsGrounded = false;
    float velocityDown = 0.0f;
    FCollisionQueryParams ignoreParams;
};