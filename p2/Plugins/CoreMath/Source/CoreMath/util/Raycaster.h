#pragma once

#include "CoreMinimal.h"

class COREMATH_API Raycaster {

public:

    bool performRaycast(
        UWorld *world, 
        const FVector &Start, 
        const FVector &dirScaled,
        FVector &outputHit
    );
    bool performRaycast(
        UWorld *world, 
        const FVector &Start, 
        const FVector &dir,
        float rayScale, 
        FVector &outputHit
    );
    bool performRaycast(
        UWorld *world,
        const FVector &Start,
        const FVector &dir,
        FCollisionQueryParams &ignoreParams,
        float rayScale,
        FVector &outputHit,
        FHitResult &outHitResult
    );
    bool performRaycast(
        UWorld *world,
        const FVector &Start,
        const FVector &dir,
        float rayScale,
        FVector &outputHit,
        FCollisionQueryParams &ignoreParams
    );

    //no outhit
    bool performRaycast(
        UWorld *world, 
        const FVector &Start, 
        const FVector &dir,
        float rayScale
    );

private:




};