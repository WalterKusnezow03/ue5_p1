#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

class IKHUMANOIDMODELL_API TwoJointBone {

public:
    TwoJointBone();
    ~TwoJointBone();

    void setup(float a, float b, UWorld *world);
    void markTriangleFlipAsWantedForArms();

    void MoveToTarget(FVector target, MMatrix &world, float deltatime);
    void MoveToTargetInverse(FVector target, float deltatime);

    void TickBuildForward(MMatrix &world, float deltatime);

    MMatrix StartEffector();
    MMatrix EndEffector();
    FVector EndEffectorLocation();

    FVector EndEffectorRelativeLocation();

    float lengthOfBone();

    FVector outOfreachDistance(FVector &target);

private:
    UWorld *worldPtr = nullptr;
    void resetRotations();

    bool markedForTriangleFlip = false; //for arms to have their elbows down
    void flipTriangleIfMarkedWanted(float &pitch1, float &pitch2);

    //size
    float length = 0.0f;

    //hip
    MMatrix t1;
    MMatrix t1Inv;
    MMatrix r1;
    MMatrix r1Inv;

    //knee
    MMatrix t2;
    MMatrix t2Inv;
    MMatrix r2;
    MMatrix r2Inv;

    //foot
    MMatrix t3;
    MMatrix t3Inv;

    //start and endeffector world saved after build
    MMatrix endEffectorWorld;
    MMatrix startEffectorWorld;

    //very important to use before using backwards kinematics
    void removeRotationFromEndEffector();

    void clampTarget(FVector &target);
    void MoveToTarget(FVector &target);
    void MoveToTargetInverse(FVector &target);

    void buildForward(MMatrix &world, float deltatime);
    void buildBackward(MMatrix &world, float deltatime);

    void draw(MMatrix &world, MMatrix &a, MMatrix &b, MMatrix &c, float dt);
};

