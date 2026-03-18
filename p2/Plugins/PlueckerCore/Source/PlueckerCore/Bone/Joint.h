#pragma once


#include "CoreMinimal.h"



#include "PlueckerCore/Math/SpatialTransforms/SpatialTransform.h"
#include "PlueckerCore/Math/SpatialTransforms/SpatialTransformBone.h"

#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Math/SpatialVector/SpatialVector.h"
#include "PlueckerCore/Interface/FJointKinematicPropagatePackage.h"
#include "GameCore/interfaces/DamageInterface/CustomHitResult.h"


/// @brief plücker DEFAULT JOINT M = R * T <--
class PLUECKERCORE_API Joint {

protected:
    bool bLogEnabled = false;
    bool disableCoriolisForce = true; //true

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

    SpatialVector &GetSpatialVelocity(){
        return GetSpatialTransform().GetSpatialVelocity();
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
    

    virtual FVector GetWorldLocation() const;






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

    


    //for force accumulation
    Matrix3x3 interia;
    Matrix3x3 interiaInverse;

    float mass = 100 * 5.0f; //1000.0 //kg * 100
    FVector centerOfMass = FVector(0.5, 0.5, 0.5); //example value

    bool BoneTranslationValidForInteriaMatrix(const FVector &size);
    void SetInteriaMatrixAuto(const FVector &boneTranslation);
    void SetInteriaMatrixSphere();


    virtual void FindSelfInteriaAndGravitySpatialMoment(
        FVector &outN, // torque
        FVector &outF // force
    );

    UWorld *world = nullptr;
    void draw(const MMatrix &a, const MMatrix &b, float deltatime);
    
    

    TArray<Joint> children;

    //all of these must be handeld externally. Litterally just pointers.
    TArray<Joint *> ChildsByPointer;
    TArray<Joint *> ParentsByPointer;
    TArray<Joint *> AllChildren();

    bool HasChildren();


    // ---- GRAVITY ----
   
public:
    
    
    void UpdateIgnoreParamsRecursive(FCollisionQueryParams &ignoreParamsIn);

protected:
    void UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn);
    void UpdateIgnoreParamsDownStream(FCollisionQueryParams &params);
    void UpdateIgnoreParamsUpStream(FCollisionQueryParams &params);

public:
    // ADD CHILD BY PTR

    /// @brief adds child by pointer, but child gets parent by pointer too!
    void AddChildByPointer(Joint *jIn);
    void AddChildsByPointer(TArray<Joint*> childs);
    void SetActor(AActor *attachActor);
    void BuildParentingRecursive();
protected:
    void AddParentByPointer(Joint *jIn);


public:
    void SetLogEnabled(bool flag);
    void SetDrawingEnabledRecursive(bool flag);
protected:
    void SetDrawingEnabled(bool flag);
    void SetDrawingEnabledDownStream(bool flag);
    void SetDrawingEnabledUpStream(bool flag);

protected:
    AActor *attachedActor = nullptr;
    
    void UpdateActorTransform();

    void PropagateWrench(float deltatime);
    void PropagateWrench(FVector &n, FVector &f, float deltatime, TArray<Joint *> &parents, float massAccumulated);
    void PropagateWrench(FVector &n, FVector &f, float deltatime, float massAccumulated);
    void AddAndIntegrateOwnSptialForce(FVector &outN, FVector &outF, float deltaTime);

    



    // center of mass recursive

protected:
    FVector CenterOfMassWorldWeightedRecursive();
    FVector CenterOfMassWorldWeightedRecursive(const FVector &root, float &sumMass);
    FVector CenterOfMassWorldWeightedRelativeTo(const FVector &pos) const;
    FVector CenterOfMassWorldRelativeTo(const FVector &pos) const;
    FVector CenterOfMassWorld() const;

    float MassOfSubTree();
    float GetTotalMassOfSubtree();
    void GetMassRecursive(float &massIn);

    // external damping

public:
    void SetAngularDampingRecursive(float factor);
protected:
    void SetAngularDamping(float factor);
};