#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/HipController/BoneAttachment.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "CoreMath/animation/TargetInterpolator.h"
#include "CoreMath/animation/RotationInterpolator.h"
#include "IkHumanoidModell/Ik/Controller/enums/ELegPhase.h"
#include "IkHumanoidModell/Ik/Controller/enums/EHipControllerStates.h"

/// @brief controls two bone attachments and runnign physics based on slip data.
/// moves the underlying hip 
class IKHUMANOIDMODELL_API HipController {

public:
    HipController();
    ~HipController();

    //default setup
    void setup(UWorld *world);

    //setup custom needed
    void setup(float legPart1, float legPart2, float offsetHip, UWorld *world);

    void Tick(float deltatime);


    //do not modify
    MMatrix &getOrientation();

    //do not modify
    MMatrix &getTranslation();


    FVector GetLocation();
    

    /// @brief places the conroller at a target, foot on ground
    void SetLocation(FVector &target);


    // --- entity api ---
    void setStateWalking();
    void stopLocomotion();

    void updateCollisionParams(FCollisionQueryParams Params);

private:
    //removed ik carried item
    FCollisionQueryParams collisionParams;

    //
    float setupLegLength = 0.0f;
    float bodyMass = 30.0f; // 10 kg ?

    float motionTime = 0.3f; // 0.7f;//0.2f;

    //actor velocity / hip velocity in every direction
    FVector velocity;

    FVector latestGroundTruth;

    UWorld *worldPointer = nullptr;

    BoneAttachment legLeft;
    BoneAttachment legRight;

    ELegPhase phaseLeft;
    ELegPhase phaseRight;

    MMatrix translation;
    MMatrix orientation;

    FVector forwardDefaultLocalLocomotionFrame;
    FVector forwardRotatedLocalLocomotionFrame;

    void buildOnStart();
    void TickLocomotion(float deltatime);
    void TickFalling(float deltatime);
    void applyLocomotion(float deltatime);
    void updateInterpolatorLocomotion(float deltatime);

    bool forwardMotion = true;
    bool legLeftPlaying = true;
    

    void updateInterpolatorLocomotion();
    void updateForwardTargetWorld(FVector &targetWorld);
    void updateBackwardTargetLocal(FVector &targetLocal);
    TargetInterpolator interpolatorForwardWorld;
    TargetInterpolator interpolatorBackwardLocal;

    TargetInterpolator interpolatorHipRotation;

    void setupBackwardInterpolation();
    void setupForwardInterpolation();

    void drawLocation(float deltatime);

    void projectToGround(FVector &worldTarjectory);

    void applyForces(float deltatime);
    void applyStancePhaseSLIPForce(float deltatime);
    void applyForceGravity(float deltatime);

    void applyVelocity(float deltatime);

    void validateTransformUpdate(FVector &position);

    bool isGrounded();
    bool groundedByDistance();

    void UpdateStanceStatus();
    bool leftInStancePhase();
    bool rightInStancePhase();

    void RebuildLegsEndInPlace(float deltatime);
    void RebuildLegsEndInPlaceFaceDown(float deltatime);

    void applyMaxVelocity();

    //debug
    FVector slipAcceleration;


    //helper for anim time
    float horizontalVelocity();
    float animationTimeBasedOnCurrentVelocity(
        FVector &localStart,
        FVector &localEnd
    );


    //new rotation
public:
    void LookAt(FVector &location);
    void setupRotationForNextStep(float radianYaw);

    /// @brief current look dir from orientation
    FVector lookDirection();

    //debug
    void forceYawAdd(float degree);

private:
    bool DEBUG_SLOWTIME = false;  // true;
    void TickHipRotation(float deltatime);
    bool anyBackwardPhase();
    RotationInterpolator hipRotationInterpolator;
    bool rotationSet = false;
    FVector forwardTrajectory();
    float slowDownTimeForDebugRotationStance(float deltatime);

    //slow down velocity on rotation change, prevent slipping
    void slowDownBasedOnRotationInRadian(float radianInNextStep);


    //locomotion state
    EHipControllerStates currentControllerState = EHipControllerStates::EIdle;
    bool locoMotionStateEnabled();
};