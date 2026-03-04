#pragma once

#include "CoreMinimal.h"

class COREMATH_API Raycaster {

public:

    bool performRaycast(
        UWorld *world, 
        const FVector &Start, 
        const FVector &dirScaled,
        FVector &outputHit,
        bool traceComplex = true
    );
    bool performRaycast(
        UWorld *world, 
        const FVector &Start, 
        const FVector &dir,
        float rayScale, 
        FVector &outputHit,
        bool traceComplex = true
    );
    bool performRaycast(
        UWorld *world,
        const FVector &Start,
        const FVector &dir,
        FCollisionQueryParams ignoreParams,
        float rayScale,
        FVector &outputHit,
        FHitResult &outHitResult,
        bool traceComplex = true
    );
    bool performRaycast(
        UWorld *world,
        const FVector &Start,
        const FVector &dir,
        float rayScale,
        FVector &outputHit,
        FCollisionQueryParams ignoreParams,
        bool traceComplex = true
    );

    //no outhit
    bool performRaycast(
        UWorld *world, 
        const FVector &Start, 
        const FVector &dir,
        float rayScale,
        bool traceComplex = true
    );

private:




};