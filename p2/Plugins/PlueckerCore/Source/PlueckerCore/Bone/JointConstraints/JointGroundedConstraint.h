#pragma once


#include "JointConstraint.h"

class PLUECKERCORE_API FJointGroundedConstraint : public FJointConstraint {
public:
    
    bool allowPitchRotationPositive = true;
    bool allowPitchRotationNegative = true;

    bool allowRollRotationPositive = true;
    bool allowRollRotationNegative = true;

    bool allowPositionOffsetZGrounded = false;
    
    virtual void ApplyRotationConstraint(FVector &rotation) const override{
        ApplyConstraintNaN(rotation);
        ApplyAboveZeroConstraint(allowPitchRotationPositive, rotation.Y);
        ApplyBelowZeroConstraint(allowPitchRotationNegative, rotation.Y);


        ApplyAboveZeroConstraint(allowRollRotationPositive, rotation.X);
        ApplyBelowZeroConstraint(allowRollRotationNegative, rotation.X);
    }

    virtual void ApplyPositionConstraint(FVector &position) const override{
        //FJointConstraint::ApplyPositionConstraint(position);
        ApplyConstraintNaN(position);
        ApplyAboveZeroConstraint(allowPositionOffsetZGrounded, position.Z);
    }
};