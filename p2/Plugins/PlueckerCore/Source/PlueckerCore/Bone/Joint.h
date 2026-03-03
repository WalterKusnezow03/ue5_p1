#pragma once


#include "CoreMinimal.h"
#include "PlueckerCore/Math/Matrix6x6.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Math/SpatialVector.h"
#include "PlueckerCore/Interface/FJointKinematicPropagatePackage.h"
#include "GameCore/interfaces/DamageInterface/CustomHitResult.h"

class PLUECKERCORE_API Joint {

protected:
    bool logEnabled = false;
    bool is6DOF = false;

public:

    Joint();
    Joint(FVector translationVector);
    Joint(FVector translationVector, UWorld *worldIn);
    ~Joint();

    Joint(const Joint &other);
    Joint &operator=(const Joint &other);

    MMatrix copyEnd();

    void MarkAs6DOF(bool flag){
        is6DOF = flag;
    }

    // add force
    //mass internal property of joint
protected:
    void AddTorque(const FVector &torque, float deltaTime);
public:
    void AddForce(const FVector &force, float deltaTime); //mass internal property of joint
    void ReactToDamage(const FCustomHitResult &hitResult);

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
    void OverrideWorldLocation(MMatrix mat);
    void OverrideWorldLocation(FVector pos);
    void OverrideJointWorldTransform(FVector pos, FRotator roation);

    MMatrix GetRotation()const{
        MMatrix result;
        spatialTransform.CopyRotationTo(result);
        return result;
    }

    // ---- external building of joints ----




    //internal build to child
    void AddChild(Joint &childIn);
    void TickAndBuildRecursive(
        float deltatime,
        FVector &w,
        FVector &v,
        MMatrix &transform
    );
    void TickAndBuildRecursive(
        float deltaTime
    );

    //as root with all torque
    void TickAndBuildRecursiveAsRoot(float deltaTime);
    void TickAndBuildRecursiveAsRoot(
        float deltaTime,
        FVector &w,
        FVector &v,
        MMatrix &transform
    );

protected:
    void TickAndBuildAll(TArray<Joint*> array, FVector w, FVector v, MMatrix root, float deltatime);
    void TickAndBuildChild(Joint &joint, FVector w, FVector v, MMatrix root, float deltatime);

public:
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
    FVector GravityTorque();
    

    UWorld *world = nullptr;
    void draw(const MMatrix &a, const MMatrix &b, float deltatime);
    void LogSpatialVelocities(FString Prefix, const FVector &w, const FVector &v);
    void LogPosition(FString Prefix);

    TArray<Joint> children;

    //all of these must be handeld externally. Litterally just pointers.
    TArray<Joint *> ChildsByPointer;
    TArray<Joint *> ParentsByPointer;
    TArray<Joint *> AllChildren();

    bool HasChildren();

    void UpdateSpatialVelocityAndPassedVelocities(FVector &w, FVector &v);

    // ---- GRAVITY EXPERIMENTAL ----
   
public:
    
    void TickGravityAndAddUpdateToSptialVector(float deltaTime);

    void UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn);
    void UpdateIgnoreParamsRecursive(FCollisionQueryParams &ignoreParamsIn);

protected:
    void UpdateIgnoreParamsDownStream(FCollisionQueryParams &params);
    void UpdateIgnoreParamsUpStream(FCollisionQueryParams &params);

public:
    // ADD CHILD BY PTR

    /// @brief adds child by pointer, but child gets parent by pointer too!
    void AddChildByPointer(Joint *jIn);
    void AddChildsByPointer(TArray<Joint*> childs);
    void SetActor(AActor *attachActor);

    void AddParentByPointer(Joint *jIn);
    void BuildParentingRecursive();

    void SetLogEnabled(bool flag);

private:
    AActor *attachedActor = nullptr;
    //t and R, in is always translation, updated always rotation.
    void UpdateActorTransform(const MMatrix &transformIn, const MMatrix &transformUpdate);

    void PropagateWrench(float deltatime);
    void PropagateWrench(FVector &n, FVector &f, float deltatime, TArray<Joint *> &parents);
    void PropagateWrench(FVector &n, FVector &f, float deltatime);
};