#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/SlipContainer.h"

class IKHUMANOIDMODELL_API BoneAttachment {

public:
    BoneAttachment();
    ~BoneAttachment();

    /// @brief will reset the bone matrices and rebuild it
    void ResetAndRebuild(
        MMatrix &translation,
        MMatrix &orientation
    );

    void setWorld(UWorld *world);
    void setAsLeg();

    /// @brief direction of bone from bottom to up, default size stretched bone.
    FVector defaultExtendedEndToStartLocal();
    
    void setupBone(float a, float b, UWorld *world, FVector offset);
    void setupBone(
        float a,
        float b,
        UWorld *worldIn,
        FVector offset,
        float massOfParent,
        float defaultMotionTime
    );

    void setForwardTargetWorld(
        FVector &targetWorld,
        MMatrix &rootTranslation,
        MMatrix &rootOrientation
    );
    void setForwardTargetLocal(FVector &target);
    void setBackwardTargetLocal(FVector &target);

    void TickNone(MMatrix &worldRoot, float deltatime);
    void TickForwardKinematic(MMatrix &transform, float deltatime);
    void TickBackwardKinematic(
        MMatrix &translationRootToUpdate, 
        MMatrix &orientationRoot,
        float deltatime
    );
    void TickKeepEndInWorldPlace(
        MMatrix &translationRoot,
        MMatrix &orientationRoot,
        float deltatime
    );
    void TickKeepEndInWorldPlaceNegHeightTrajectory(
        MMatrix &translationRoot,
        MMatrix &orientationRoot,
        float deltatime
    );

    void TickForwardKinematicOutOfReachTarget(
        MMatrix &translation,
        MMatrix &orientation, // könnte temporäre kopie sein
        float deltatime
    );

    ///@brief projects a world trajectory into local space, derotated,
    ///into the start effector space of the bone (Inner offset removed.)
    FVector inLocalSpace(
        FVector &worldFrame,
        MMatrix &rootTranslation,
        MMatrix &rootOrientation);
    FVector inWorldSpace(
        FVector &localFrame,
        MMatrix &rootTranslation,
        MMatrix &rootOrientation);
    FVector hipRelativeLocationToEndEffector(
        MMatrix &rootTranslation,
        MMatrix &rootOrientation);

    FVector endEffectorWorldLocation();

    SlipContainer &slipData(MMatrix &orientation);

    bool reachedTarget();
    bool reachedTargtZ();
    float distanceFromTarget();

    //experimental
    void updateLocalForwardMovingDirection(FVector &localMove);
    void resetLocalForwardMovingDirection();


    //api for set damaged owner
    void getActors(TArray<AActor *> &outArray);

private:
    FVector defaultExtendedTranslationBottomToUp;

    FVector localMovingDirectionSaved;

    MMatrix innerOffset;
    MMatrix innerOffsetInverse;

    TwoJointBone bone;

    FVector forwardTargetWorld;
    FVector forwardTarget;
    FVector backwardTarget;

    MMatrix startEffectorTransformWorld(MMatrix &worldRoot);

    UWorld *world = nullptr;
    bool bDebugDraw = true;

    SlipContainer container;

public:
    /// @brief pre calculates thee slip scalar D for slip force, based on the 
    /// current end effector location and a dynamically calculated lift off frame
    /// the lift off frame is currently hacked from the current end effector location,
    /// another overloaded method will fix this issue.
    /// @param orientation 
    /// @param localEnd 
    /// @param time 
    /// @param velocityDown 
    /// @param mass 
    void setupSlipDataOnStanceBegin(
        MMatrix &orientation,
        FVector &defaultForwardFrame,
        float time,
        float velocityDown,
        float mass
    );



    void setupSlipDataOnStanceBegin(
        MMatrix &orientation,
        MMatrix &translation,
        FVector &nextTrajectoryOfOtherLegWorldSpace, //next projceted frame of next leg target, !!velocity removed!!
        float time,
        float velocityDown,
        float mass
    );






};
