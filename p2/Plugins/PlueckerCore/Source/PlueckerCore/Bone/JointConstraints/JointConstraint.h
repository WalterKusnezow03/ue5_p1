#pragma once

#include "CoreMinimal.h"
#include "JointConstraintBase.h"

class PLUECKERCORE_API FJointConstraint : public FJointConstraintBase{
    
public:
    //default setup does not allow position, but all degrees of freedom for rotation
    bool allowRollRotation = true;
    bool allowPitchRotation = true;
    bool allowYawRotation = true;

    
    bool allowPositionOffsetX = false;
    bool allowPositionOffsetY = false;
    bool allowPositionOffsetZ = false;
   


    virtual void ApplyRotationConstraint(FVector &rotation) const{
        ApplyConstraint(allowRollRotation, rotation.X);
        ApplyConstraint(allowPitchRotation, rotation.Y);
        ApplyConstraint(allowYawRotation, rotation.Z);
    }

    virtual void ApplyPositionConstraint(FVector &pos) const { 
        ApplyConstraint(allowPositionOffsetX, pos.X);
        ApplyConstraint(allowPositionOffsetY, pos.Y);
        ApplyConstraint(allowPositionOffsetZ, pos.Z);
    }

    void Mark6DOFJoint(){
        allowRollRotation = true;
        allowPitchRotation = true;
        allowYawRotation = true;
        allowPositionOffsetX = true;
        allowPositionOffsetY = true;
        allowPositionOffsetZ = true;
    }


    void UnLockPositionConstraint(){
        allowPositionOffsetX = true;
        allowPositionOffsetY = true;
        allowPositionOffsetZ = true;
    }

    void LockPositionConstraint(){
        allowPositionOffsetX = false;
        allowPositionOffsetY = false;
        allowPositionOffsetZ = false;
    }

private:
    


    



};
