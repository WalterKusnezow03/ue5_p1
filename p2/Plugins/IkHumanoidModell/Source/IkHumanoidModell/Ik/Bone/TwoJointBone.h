#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

class IKHUMANOIDMODELL_API TwoJointBone {

public:
    TwoJointBone();
    ~TwoJointBone();

    void setup(float a, float b, UWorld *world);
    void markTriangleFlipAsWantedForArms();
    void markTriangleFlipAsWantedForLegs();

    //move to target and build
    void MoveToTarget(FVector target, MMatrix &world, float deltatime);
    void MoveToTargetInverse(FVector target, float deltatime);

    //new
    void MoveToTarget(
        FVector target,
        MMatrix &world,
        float deltatime,
        FVector &localForward //forward direction to prevent unexpected yaw and use roll instead.
    );

    /// @brief builds the boen without any fk ik 
    /// @param world 
    /// @param deltatime 
    void TickBuildForward(MMatrix &world, float deltatime);

    /// @brief start effector WORLD Matrix
    /// @return 
    MMatrix StartEffector();

    /// @brief endeffector WORKD MATRIX
    /// @return 
    MMatrix EndEffector();

    /// @brief end effector world location
    /// @return 
    FVector EndEffectorLocation();

    /// @brief end effector world matrix without rotational part
    /// @return 
    MMatrix EndEffectorTranslation();

    /// @brief end effector location relative to start effector in world space (StartToEndEffector vector)
    /// @return 
    FVector EndEffectorRelativeLocation();

    float lengthOfBone();

    /// @brief scaled vector of a size if out of reach target
    /// @param target 
    /// @return 
    FVector outOfreachDistance(FVector &target);

    void clampTarget(FVector &target);
    void clampToFullMotionRangeCircle(FVector &target);
    bool targetIsInMotionCircle(FVector &target);

    // -- api for layered bone --

    /// @brief inverse transform of the whole bone
    /// @return 
    MMatrix inverseTransform();
    void overrideR2KneeRotation(MMatrix &rin);

    void useOtherColorType();

private:
    bool bLogEnabled = true;
    bool bDrawBackwardReach = false;
    bool bDrawToWorldStart = false;

    bool colorFlagChanged = false;

    UWorld *worldPtr = nullptr;
    void resetRotations();

    bool markedForTriangleFlip = false; //for arms to have their elbows down

    bool markedForTriangleflipArms = false;
    bool markedForTriangleflipLegs = false;
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

    
    void MoveToTarget(FVector &target);
    void MoveToTargetInverse(FVector &target);
    void copyRotationFromInverseMatrices();

    void buildForward(MMatrix &world, float deltatime);
    void buildBackward(MMatrix &world, float deltatime);

    void draw(MMatrix &world, MMatrix &a, MMatrix &b, MMatrix &c, float dt);


    //new
    void MoveToTarget(FVector &target, FVector &localForward);
};
