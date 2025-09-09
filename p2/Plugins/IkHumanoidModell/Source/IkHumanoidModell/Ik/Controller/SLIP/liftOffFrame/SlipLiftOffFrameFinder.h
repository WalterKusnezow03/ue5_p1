#pragma once

#include "CoreMinimal.h"

class IKHUMANOIDMODELL_API SlipLiftOffFrameFinder {

public:
    SlipLiftOffFrameFinder();
    ~SlipLiftOffFrameFinder();

    

    FVector FindLiftOffFrameRelativeLocalToNextHipFromWorldTrajectories(
        FVector &worldCurrentFootPosTarget, // Target of Leg 1, currently for forward motion reached!
        FVector &worldNextFootPosTarget,    // Target of Leg 2, next world Trajectory
        float boneLength,
        float timeOfAnimation,
        FVector2D &velocityHorizontal
    );

    static void Test();


    //--- in use currently, stable ---
    FVector FindLiftOffFrameRelativeLocalToNextHipFromWorldTrajectories(
        FVector &worldCurrentFootPosTarget, // A0 (current foot target / "A_1" in your notes)
        FVector &worldNextFootPosTarget,    // B0 (next foot target)
        float boneLength,
        float heelOffEpsilon /* = 10.0f */ // make epsilon configurable
    );


    FVector FindRelativeLiftOffFrameBasedOnHeight(
        FVector &worldFootA,
        FVector &worldFootB,
        float lengthOfBone,
        float heelOffEpsilon
    );

private:
    void LogResult(
        FString prefix,
        float closerTSkalar,
        FVector &localResult //local space to hip
    );

    void LogResultLocalSpace(
        FString prefix,
        FVector &direction,
        float closerTSkalar,
        FVector &localResult //local space to hip
    );
};