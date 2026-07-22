#pragma once

#include "CoreMinimal.h"

/// @brief will manage the forward and backward kinematic trajectory times
/// for the hipcontroller
class IKHUMANOIDMODELL_API AnimationTime {

public:
    static float AnimationTimeBasedOnHorizontalVelocity(
        const FVector &a,
        const FVector &b,
        float horizontalVelocity,
        float minMotionTime
    );

    static float AnimationTimeBasedOnHorizontalAndVerticalVelocity(
        const FVector &localStart,
        const FVector &localEnd,
        float horizontalVelocity,
        float verticalVelocity,
        float minMotionTime
    );

    static float AnimationTimeBasedOnHorizontalAndVerticalVelocity(
        const FVector &localStart,
        const FVector &localEnd,
        float horizontalVelocity,
        float verticalVelocity
    );

private:
    AnimationTime(){}
    ~AnimationTime(){}


};