#pragma once


#include "JointConstraint.h"

class PLUECKERCORE_API FJointGroundedConstraint : public FJointConstraint {
public:
    
    bool allowPitchRotationPositive = true;
    bool allowPitchRotationNegative = true;

    bool allowRollRotationPositive = true;
    bool allowRollRotationNegative = true;

    bool allowPositionOffsetZGrounded = true;
    
    virtual void ApplyRotationConstraint(FVector &rotation) const override{
        ApplyConstraintNaN(rotation);
        ApplyAboveZeroConstraint(allowPitchRotationPositive, rotation.Y);
        ApplyBelowZeroConstraint(allowPitchRotationNegative, rotation.Y);


        ApplyAboveZeroConstraint(allowRollRotationPositive, rotation.X);
        ApplyBelowZeroConstraint(allowRollRotationNegative, rotation.X);
    }

    virtual void ApplyPositionConstraint(FVector &velocity) const override{
        //FJointConstraint::ApplyPositionConstraint(position);
        ApplyConstraintNaN(velocity);
        ApplyAboveZeroConstraint(allowPositionOffsetZGrounded, velocity.Z);
    }

    FString ToString(){
        FString flag = allowPositionOffsetZGrounded ? TEXT("Z ALLOWED") : TEXT("Z NOT! ALLOWED");
        FString result = FString::Printf(
            TEXT("FJointGroundedConstraint:: Z Constraint: (%s)"),
            *flag
        );
        return result;
    }
};