#pragma once


#include "CoreMinimal.h"
#include "PlueckerCore/Math/Matrix6x6.h"
#include "CoreMath/Matrix/MMatrix.h"

class PLUECKERCORE_API Joint {

public:
    
    Joint();
    Joint(FVector translationVector);
    Joint(FVector translationVector, UWorld *worldIn);
    ~Joint();

    Joint(const Joint &other);
    Joint &operator=(const Joint &other);

    void AddChild(Joint &childIn);

    
    void Tick(float deltaTime, FVector &w, FVector &v);
    void Build(MMatrix &inTransform);

    void SetWorld(UWorld *worldIn);

    void DrawJointLocation(float deltatime);

    void SetDrawColor(FColor colorIn);
    void SetDrawColorRecursive(FColor colorA, FColor colorB, int i);

    void SetBoneTranslationDirection(FVector direction);
    FVector BoneTranslationDirection()const ;

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