#pragma once

#include "CoreMinimal.h"


class PLUECKERCORE_API FNormalForce {

public: 
    

    static FVector NormalForce(
        const FVector &groundNormal, 
        float groundPenetration,
        const FVector &velocity
    ){
        //debug
        //groundPenetration += 10.0f; //10cm extra.

        //stable values, do not change
        float feather = 5000.0f;        // Federkonstante
        feather *= 2.0f;

        float damp = 50.0f;         // Dämpfung
        damp *= 3.0f;

        float penetrationVelocity = FVector::DotProduct(velocity, groundNormal);
        float scale = (feather * groundPenetration - damp * penetrationVelocity);

        //no downwards normal force.
        scale = std::max(scale, 0.0f);

        FVector FNormal = scale * groundNormal.GetSafeNormal();
        return FNormal;
        
        
        
    }



};
