#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "IkHumanoidModell/Ik/Bone/ETwoBoneConstraint.h"

#include "PlueckerCore/Bone/Joint.h"
#include "PlueckerCore/Interface/IJointInterface.h"


#include "IkHumanoidModell/Ik/Bone/JointCache/JointTransformCache.h"
#include "IkHumanoidModell/Ik/Bone/BoneTransformInterface/BoneTransformInterface.h"

class FTwoLimbProperty;

class IKHUMANOIDMODELL_API TwoJointBone : public IJointInterface, public IBoneTransformInterface{

private:
    bool bDrawLines = false; //false

    bool aactorBasedBones = true; //much faster than component based!!

public:
    TwoJointBone();
    ~TwoJointBone();

    void ResetAndRebuild(MMatrix &worldMatrix);

    void setup(FTwoLimbProperty &property);

    

    //-- to be marked protected! --
    void setup(float a, float b, UWorld *world);
    void setup(float a, float b, UWorld *world, float widthBone); //hand controller api

protected:
    FVector DownVector(float a);
    void setupMatrices(float a, float b, UWorld *world);

public:
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

    /// @brief endeffector WORLD MATRIX
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

    void getActors(TArray<AActor *> &outArray);

    void OverrideEndEffectorWorldLocation(FVector &location);

    // --- ell to end direction for ikcarryinterface ---
    const FVector &DirectionOfMiddleToEndEffector(){
        return directionOfMiddleToEndEffectorSaved;
    }

private:
    FVector directionOfMiddleToEndEffectorSaved;
    void UpdateDirectionOfMiddleToEndEffector(MMatrix &middle, MMatrix &end);

private:
    bool bLogEnabled = false;
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

    //new not tested
    void FlipTriangleBasedOnConstraint(float &pitch1, float &pitch2, FVector targetLocal);
    void FlipTriangleBasedOnConstraint(float &pitch1, float &pitch2, float angleToForwardLocal);

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
    void draw(MMatrix &a, MMatrix &b, float dt, FColor color);
    void GetColorsForDraw(FColor &colorA, FColor &colorB, FColor &colorC);

    //Use this method for leg FK! 
    void MoveToTarget(FVector &target, FVector &localForward);



    // --- api for actor attachment ---
public:
    void attachLimbs(AActor *top, AActor *bottom);

    void SetConstraint(ETwoBoneConstraint constraintIn){
        constraint = constraintIn;
    }

private:
    bool autoCreateLimbs = true;
    void createLimbsIfNeeded(UWorld *world, float a, float b);
    void createLimbsIfNeeded(UWorld *world, float a, float b, float widthBone);

    AActor *topActor = nullptr;
    AActor *bottomActor = nullptr;
    void applyTransformToActors(MMatrix &world, MMatrix &top, MMatrix &bottom);
    void applyTransform(AActor *ptr, FVector location, MMatrix &rotationMatrix);

    ETwoBoneConstraint constraint = ETwoBoneConstraint::ENone;


    // --- scene component based bones ---
public:
    virtual bool GetTransform(
        FVector &location,
        FRotator &rotation,
        int limbId
    ) override;

protected:
    virtual bool IdIsValid(int id) override;
    void CreateLimbs(FTwoLimbProperty &property);

    void UpdateJointTransformCaches(
        MMatrix &world, // hip world
        MMatrix &top,   // knee world
        MMatrix &bottom // foot world
    );
    void UpdateJointTransformCache(
        JointTransformCache &cache,
        FVector location,
        MMatrix &rotationTransform
    );
    JointTransformCache hipWorldCached;
    JointTransformCache kneeWorldCached;
    JointTransformCache footWorldCached;

    // --- pluecker joints ---
public:
    virtual void UpstreamPropagate(FJointKinematicPropagatePackage &package) override;

    virtual void DownstreamPropagate(
        FJointKinematicPropagatePackage &package
    )override;

    void setupPlueckerJoints(float a, float b, UWorld * world);
    Joint p1; //hip to knee
    Joint p2; //knee to foot

    Joint p2Invert; //foot to knee
    Joint p1Invert; //knee to foot

    //transform to plucker
    void UpdatePluckerJointsFromCurrentJoints();
    //plucker to inverted plucker
    void UpdateInvertedPluckerJointsFromCurrentPluckerJoints();
};
