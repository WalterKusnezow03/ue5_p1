#pragma once

#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "EArmType.h"

/// @brief will just abstract a part of the torso and follow targets,
/// maybe applies IK to lift the skelleton up. Unclear.
class IKHUMANOIDMODELL_API LayeredTwoJointBone {

public:
    LayeredTwoJointBone();
    ~LayeredTwoJointBone();

    /// @brief sets up the internal bones for layered fk ik
    void setup(
        EArmType typeArm,
        float hipBreast,
        float breastShoulder,
        float upperArm,
        float lowerArm,
        UWorld *worldIn
    );

    // ---- TICK SECTION ----
    
    ///@brief will tick the bone(s) and apply fk to target if needed (e.x. aactor attached to follow hand)
    void Tick(
        MMatrix &actorTranslation,
        MMatrix &actorRotation,
        float deltatime
    );

    /// @brief tries to reach the world target
    /// @param actorTranslation 
    /// @param actorRotation 
    /// @param deltatime 
    void TickForwardKinematicsWorldTarget(
        MMatrix &actorTranslation,
        MMatrix &actorRotation, 
        float deltatime
    );

    //builds bones without using fk / ik
    void TickBuildNone(
        MMatrix &actorTranslation,
        MMatrix &actorRotation,
        float deltatime
    );

    /// @brief maybe needed to be updated every frame, local target is followed (?)
    /// @param target 
    void setWorldTarget(FVector &target);
    void setLocalTarget(FVector &target);

    MMatrix startEffectorRotation(); //of hip

    //might be switched to an Interface from game core to have an more precise API! - TODO
    void dropCarriedItem();
    void attachOrReplaceCarriedItem(IIkCarryInterface *actor);

private:
    EArmType armTypeSaved;
    TwoJointBone torsoBone;
    TwoJointBone armBone;

    //todo whether a local or a world target is tracked
    IIkCarryInterface *attachedItemInterface = nullptr;

    FVector worldTarget;
    FVector localTarget;

    UWorld *world = nullptr;

    float totalMotionCircleSize = 0.0f;

    //calculates the full motion available circle size
    void findTotalMotionCircleSize(
        float hipTorso, 
        float torsoShoulder, 
        float armUpper, 
        float armLower
    );

    /// @brief will tell if the target is in the motion cirle of torso and arm
    /// @param target 
    /// @return 
    bool localTargetInMotionCircle(FVector &target);

    /// @brief will tell if the target is in motion circle of arm
    /// @param target 
    /// @return 
    bool localTargetInArmMotionCircle(FVector &target);

    /// @brief clamps a world target to the full motion circle available
    /// @param actorTranslation 
    /// @param targetWorld 
    /// @return 
    FVector clampToFullMotionCircleWorld(
        MMatrix &actorTranslation,
        FVector &targetWorld
    );

    /// @brief moves target into local space of arm
    /// @param actorTranslation 
    /// @param actorOrientation 
    /// @param worldTarget 
    /// @return 
    FVector moveToLocalSpace(
        MMatrix &actorTranslation,
        MMatrix &actorOrientation, 
        FVector &worldTarget
    );

    //torso raw build
    void overrideTorsoBreastRotationDefault();
    void buildTorsoBoneNone(
        MMatrix &actorTransform,
        float deltatime
    );

    /// @brief find the local target of the shoulder, use when local target out local
    /// arm motion circle, layered ik here. 
    /// @param targetWorld 
    /// @param targetLocalArm 
    /// @param actorTranslation 
    /// @param actorRotation 
    /// @return 
    FVector shoulderLocalTargetFromTargetArmIfOutOfRange(
        FVector &targetWorld,
        FVector &targetLocalArm,
        MMatrix &actorTranslation,
        MMatrix &actorRotation
    );




    //actor attachment
    bool itemIsAttached();
    FVector HandTargetWorldBasedOnAttachedItem();

};