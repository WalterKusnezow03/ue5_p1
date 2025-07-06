#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/BoneAttachment.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "CoreMath/animation/TargetInterpolator.h"
#include "CoreMath/animation/RotationInterpolator.h"
#include "IkHumanoidModell/Ik/Controller/enums/ELegPhase.h"

class IKHUMANOIDMODELL_API HipController {

public:
    HipController();
    ~HipController();

    void setup(UWorld *world);

    void Tick(float deltatime);

    void setLocation(FVector &location);

private:
    float setupLegLength = 0.0f;
    float bodyMass = 30.0f; // 10 kg ?

    float motionTime = 1.0f; // 0.7f;//0.2f;

    //actor velocity
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
    void setupRotationForNextStep(float radianYaw);

private:    
    
    void TickHipRotation(float deltatime);
    bool anyBackwardPhase();
    RotationInterpolator hipRotationInterpolator;
    bool rotationSet = false;
};