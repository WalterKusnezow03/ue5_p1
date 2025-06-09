#pragma once

#include "CoreMinimal.h"

#include "TwoBone.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "CoreMath/animation/TargetInterpolator.h"

class P2_API WalkingController {

public:
    WalkingController();
    ~WalkingController();

    void Tick(float deltatime, TwoBone &right, TwoBone &left);

private:
    MMatrix &endEffectorRight;
    MMatrix &endEffectorLeft;

    
    TargetInterpolator interpolator;
};