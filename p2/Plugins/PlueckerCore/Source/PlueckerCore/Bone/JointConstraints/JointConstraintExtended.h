#pragma once

#include "JointConstraint.h"

class PLUECKERCORE_API FJointConstraintExtended {
    bool allowRollRotationPositive = true;
    bool allowPitchRotationPositive = true;
    bool allowYawRotationPositive = true;

    bool allowRollRotationNegative = true;
    bool allowPitchRotationNegative = true;
    bool allowYawRotationNegative = true;

};