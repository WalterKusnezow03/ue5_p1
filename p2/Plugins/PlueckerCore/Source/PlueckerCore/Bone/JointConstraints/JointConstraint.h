#pragma once

#include "CoreMinimal.h"

class PLUECKERCORE_API FJointConstraint {
    
public:
    //default setup does not allow position, but all degrees of freedom for rotation
    bool allowRollRotation = true;
    bool allowPitchRotation = true;
    bool allowYawRotation = true;

    
    bool allowPositionOffsetX = false;
    bool allowPositionOffsetY = false;
    bool allowPositionOffsetZ = false;
   


    void ApplyRotationConstraint(FVector &rotation) const{
        ApplyConstraint(allowRollRotation, rotation.X);
        ApplyConstraint(allowPitchRotation, rotation.Y);
        ApplyConstraint(allowYawRotation, rotation.Z);

    }

    void ApplyPositionConstraint(FVector &pos) const { 
        ApplyConstraint(allowPositionOffsetX, pos.X);
        ApplyConstraint(allowPositionOffsetY, pos.Y);
        ApplyConstraint(allowPositionOffsetZ, pos.Z);
    }

private:
    void ApplyConstraint(bool flag, double &num)const{
        if(!flag){
            num = 0.0;
        }
    }



};
