#pragma once

#include "CoreMinimal.h"

class PLUECKERCORE_API FJointConstraintBase {
    
public:
   
    

protected:
    void ApplyConstraint(bool flag, double &num)const{
        if(!flag){
            num = 0.0;
        }
    }

    void ApplyBelowZeroConstraint(bool flag, double &num)const{
        if(!flag){
            if(num < 0.0f){
                num = 0.0f;
            }
        }
    }
    void ApplyAboveZeroConstraint(bool flag, double &num)const{
        if(!flag){
            if(num > 0.0f){
                num = 0.0f;
            }
        }
    }


};
