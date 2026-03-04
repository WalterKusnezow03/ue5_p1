#include "Raycaster.h"

bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dirScaled,
    FVector &outputHit,
    bool traceComplex
){
    return performRaycast(
        world, 
        Start, 
        dirScaled,
        1.0f, 
        outputHit,
        traceComplex
    );
}


bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dir,
    float rayScale, 
    FVector &outputHit,
    bool traceComplex
){
    FHitResult empty;
    FCollisionQueryParams ignoreParams;
    return performRaycast(
        world,
        Start,
        dir,
        ignoreParams,
        rayScale,
        outputHit,
        empty,
        traceComplex
    );
}

bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dir,
    float rayScale, 
    FVector &outputHit,
    FCollisionQueryParams ignoreParams,
    bool traceComplex
){
    FHitResult none;
    return performRaycast(
        world, 
        Start, 
        dir,
        ignoreParams,
        rayScale,
        outputHit,
        none,
        traceComplex
    );
}

bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dir,
    float rayScale,
    bool traceComplex
){
    FVector ignoredOutput;
    return performRaycast(
        world, 
        Start, 
        dir,
        rayScale, 
        ignoredOutput,
        traceComplex
    );
}

bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dir,
    FCollisionQueryParams ignoreParams,
    float rayScale,
    FVector &outputHit,
    FHitResult &outHitResult,
    bool traceComplex
){
    if(world == nullptr){
        return false;
    }
    ignoreParams.bTraceComplex = traceComplex; //lower complexity if false

    rayScale = std::abs(rayScale);
	FVector End = Start + dir * rayScale; // gx = A + r (B - A)
	bool bHit = world->LineTraceSingleByChannel(outHitResult, Start, End, ECC_Visibility, ignoreParams);
	
	if (bHit){
		outputHit = outHitResult.ImpactPoint; //write impactpoint to output
		return true;
	}

	return false;
}






    
