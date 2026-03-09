#pragma once


#include "CoreMinimal.h"


#include "PlueckerCore/Math/Matrix6x6.h"
#include "PlueckerCore/Math/SpatialTransforms/SpatialTransform.h"
#include "PlueckerCore/Math/SpatialTransforms/SpatialTransformBone.h"

#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Math/SpatialVector.h"
#include "PlueckerCore/Interface/FJointKinematicPropagatePackage.h"
#include "GameCore/interfaces/DamageInterface/CustomHitResult.h"


/// @brief plücker DEFAULT JOINT M = R * T <--
class PLUECKERCORE_API Joint {

protected:
    bool logEnabled = false;
    
    //--- spatial transform -- overriden in root --
private:
    SpatialTransformBone spatialTransformBase;

protected:
    virtual SpatialTransform &GetSpatialTransform(){
        return spatialTransformBase;
    }

    virtual const SpatialTransform &GetSpatialTransformConst() const {
        return spatialTransformBase;
    }


public:

    Joint();
    Joint(FVector translationVector);
    Joint(FVector translationVector, UWorld *worldIn);
    virtual ~Joint();

    Joint(const Joint &other);
    Joint &operator=(const Joint &other);


protected:
    void Setup(FVector &location);
    void Setup(FVector &location, UWorld *worldIn);

    // add force
    // mass internal property of joint
protected:
    
public:
    void AddForce(const FVector &force, float deltaTime); //mass internal property of joint
    void ReactToDamage(const FCustomHitResult &hitResult);

    // ---- external building of joints ----

    


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
    void OverrideJointRotation(const FRotator &r);
    void OverrideJointRotation(const MMatrix &rotationMatrix);
    virtual void OverrideJointWorldTransform(FVector pos, FRotator roation);

    void OverrideWorldLocation(MMatrix mat);
    virtual void OverrideWorldLocation(FVector pos);







    MMatrix GetRotation() const;
    // ---- external building of joints ----

    //internal build to child
    void AddChild(Joint &childIn);
    void TickAndBuildRecursive(
        float deltatime,
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


    
    FJointConstraint &GetConstraint();

   

    //marked as false as expected
    void logGroundedState(FString prefix, FColor trueColor);

protected:
    
    virtual void SetInteriaMatrixAuto();

    FColor color = FColor::Red;

    MMatrix transformCopy;

    //saves the current w and v of this joint, allows to add torque and force.
    SpatialVector spatialVelocity;

    //for force accumulation
    Matrix3x3 interia;
    Matrix3x3 interiaInverse;

    float mass = 100 * 5.0f; //1000.0 //kg * 100
    FVector centerOfMass = FVector(0.5, 0.5, 0.5); //example value

    bool BoneTranslationValidForInteriaMatrix(const FVector &size);
    void SetInteriaMatrixAuto(const FVector &boneTranslation);
    void SetInteriaMatrixAngularVelocityLocked();


    void FindSelfInteriaAndGravitySpatialMoment(
        FVector &outN, // torque
        FVector &outF // force
    );

    UWorld *world = nullptr;
    void draw(const MMatrix &a, const MMatrix &b, float deltatime);
    
    void LogPosition(FString Prefix);

    TArray<Joint> children;

    //all of these must be handeld externally. Litterally just pointers.
    TArray<Joint *> ChildsByPointer;
    TArray<Joint *> ParentsByPointer;
    TArray<Joint *> AllChildren();

    bool HasChildren();

    void AddOwnJointVelociyTo(FVector &w, FVector &v);

    // ---- GRAVITY ----
   
public:
    
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

protected:
    AActor *attachedActor = nullptr;
    //t and R, in is always translation, updated always rotation.
    void UpdateActorTransform(const MMatrix &transformIn, const MMatrix &transformUpdate);

    void PropagateWrench(float deltatime);
    void PropagateWrench(FVector &n, FVector &f, float deltatime, TArray<Joint *> &parents);
    void PropagateWrench(FVector &n, FVector &f, float deltatime);
    void AddAndIntegrateOwnSptialForce(FVector &outN, FVector &outF, float deltaTime);

    
};