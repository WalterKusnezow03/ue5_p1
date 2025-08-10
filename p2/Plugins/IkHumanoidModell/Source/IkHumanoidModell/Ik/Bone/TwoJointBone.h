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

    //move the end effector to a target and build
    void MoveToTarget(FVector target, MMatrix &world, float deltatime);

    //moves the start effector to a target and builds
    void MoveToTargetInverse(FVector target, float deltatime);

    //USE THIS METHOD FOR LEG FK IK! Includes local forward direction
    //to use roll rotation instead of yaw, creates more believable and less
    //glitchy movement when following a trajectory!!
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

    //clamps a target to the motion sphere if needed
    void clampTarget(FVector &target);

    //sets the local target to be on the motion sphere, whether inside or outside.
    void clampToFullMotionRangeCircle(FVector &target);

    //returns if a local target is reachable
    bool targetIsInMotionCircle(FVector &target);

    // --- api for layered bone ---

    /// @brief inverse transform of the whole bone, not needed!
    /// @return 
    MMatrix inverseTransform();

    /// can override the knee rotation for outside manipulation, 
    /// may be used for torso but is not.
    void overrideR2KneeRotation(MMatrix &rin);

    /// use other colors for debugging (used for torso bones)
    void useOtherColorType();


private:
    bool bLogEnabled = true;
    bool bDrawBackwardReach = false;
    bool bDrawToWorldStart = false;

    bool colorFlagChanged = false;

    UWorld *worldPtr = nullptr;

    //resets the rotation matrices
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


    //Use this method for leg FK! 
    void MoveToTarget(FVector &target, FVector &localForward);



    // --- api for actor attachment ---
public:
    void attachLimbs(AActor *top, AActor *bottom);

private:
    bool autoCreateLimbs = true;
    void createLimbsIfNeeded(UWorld *world, float a, float b);

    AActor *topActor = nullptr;
    AActor *bottomActor = nullptr;
    void applyTransformToActors(MMatrix &world, MMatrix &top, MMatrix &bottom);
    void applyTransform(AActor *ptr, FVector location, MMatrix &rotationMatrix);
};
