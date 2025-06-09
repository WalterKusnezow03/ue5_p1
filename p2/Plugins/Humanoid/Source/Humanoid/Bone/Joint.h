#pragma once


#include "CoreMinimal.h"
#include "Humanoid/Math/Matrix6x6.h"
#include "CoreMath/Matrix/MMatrix.h"

class HUMANOID_API Joint {

public:
    static void resetId(){
        id = 0;
    }

    Joint(UWorld *world);
    Joint(UWorld *world, FVector &translationVector);
    ~Joint();

    Joint(Joint &other);
    Joint &operator=(Joint &other);

    void SetChild(Joint *childIn);
    void Tick(float deltaTime, FVector &torqueIn); //OLD
    void Tick(float deltaTime, FVector &w, FVector &v);
    void Build(MMatrix &inTransform);

    MMatrix endTransform();

    bool isTwoBone();
    int childCountDownstream();

    FVector T_vonW(
        MMatrix &transform, 
        FVector &angularVelocity,
        float deltatime
    );

private:
    

    MMatrix transformCopy;

    void copyDeltatime(float deltaTime);

    static int id;
    int ownId = 0;

    FVector torque;

    FVector angularAcceleration;
    FVector angularVelocity;
    FVector anglesIntegrated;

    //start
    void addTorque(FVector &torqueIn);

    void recalculateRotationMatrix(float deltatime);
    void recalculateAngularAcceleration();
    void recalculateAngualarVelocity(float deltatime);
    float skalarAngualarAcceleration(float torqueVal, float interia);

    Matrix6x6 spatialTransform;

    float deltatime;

    Joint *child = nullptr;
    UWorld *world = nullptr;
    void draw(MMatrix &a, MMatrix &b);
};