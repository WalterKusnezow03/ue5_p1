#pragma once


#include "CoreMinimal.h"
#include "PlueckerCore/Math/Matrix6x6.h"
#include "PlueckerCore/Bone/JointConstraints/JointConstraint.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Interface/FJointKinematicPropagatePackage.h"

class PLUECKERCORE_API Joint {

public:
    
    Joint();
    Joint(FVector translationVector);
    Joint(FVector translationVector, UWorld *worldIn);
    ~Joint();

    Joint(const Joint &other);
    Joint &operator=(const Joint &other);



    // ---- external building of joints ----

    //external build, external propagation
    //returns updated transform
    MMatrix TickAndBuildThisJoint(FJointKinematicPropagatePackage &package);
    MMatrix TickAndBuildThisJoint(
        float deltaTime,
        FVector &w,
        FVector &v,
        const MMatrix &inTransform
    );
    // external build, external propagation

    // external transform updates
    void OverrideJointRotation(const MMatrix &rotationMatrix);
    void OverrideJointRotationTransposed(const Joint &other);

    // ---- external building of joints ----




    //internal build to child
    void AddChild(Joint &childIn);
    void Tick(float deltaTime, FVector &w, FVector &v);
    void Build(MMatrix &inTransform);
    //internal build to child

    void SetWorld(UWorld *worldIn);

    void DrawJointLocation(float deltatime);

    void SetDrawColor(FColor colorIn);
    void SetDrawColorRecursive(FColor colorA, FColor colorB, int i);

    void SetBoneTranslationDirection(FVector direction);
    FVector BoneTranslationDirection()const ;


    void OverrideConstraint(FJointConstraint &in);
    FJointConstraint &GetConstraint();

private:
    FColor color = FColor::Red;

    MMatrix transformCopy;
    Matrix6x6 spatialTransform;

    void copyDeltatime(float deltaTime);
    float deltatime;

    UWorld *world = nullptr;
    void draw(MMatrix &a, MMatrix &b);

    TArray<Joint> children;
};