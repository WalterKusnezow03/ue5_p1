#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/BoneAttachment.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "CoreMath/animation/TargetInterpolator.h"

class IKHUMANOIDMODELL_API HipController {

public:
    HipController();
    ~HipController();

    void setup(UWorld *world);

    void Tick(float deltatime);

    void setLocation(FVector &location);

private:
    UWorld *worldPointer = nullptr;

    BoneAttachment legLeft;
    BoneAttachment legRight;

    MMatrix translation;
    MMatrix orientation;

    FVector forwardDefaultLocalLocomotionFrame;

    void buildOnStart();
    void TickLocomotion(float deltatime);
    void applyLocomotion(float deltatime);
    void updateInterpolatorLocomotion(float deltatime);

    float motionTime = 1.0f;

    bool forwardMotion = true;
    bool legLeftPlaying = true;

    void updateInterpolatorLocomotion();
    void updateForwardTargetWorld(FVector &targetWorld);
    void updateBackwardTargetLocal(FVector &targetLocal);
    TargetInterpolator interpolatorForwardWorld;
    TargetInterpolator interpolatorBackwardLocal;

    void setupBackwardInterpolation();
    void setupForwardInterpolation();

    void drawLocation(float deltatime);
};