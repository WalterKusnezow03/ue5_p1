#include "Raycaster.h"

bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dirScaled,
    FVector &outputHit
){
    return performRaycast(
        world, 
        Start, 
        dirScaled,
        1.0f, 
        outputHit
    );
}


bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dir,
    float rayScale, 
    FVector &outputHit
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
        empty
    );
}





bool Raycaster::performRaycast(
    UWorld *world, 
    const FVector &Start, 
    const FVector &dir,
    FCollisionQueryParams &ignoreParams,
    float rayScale,
    FVector &outputHit,
    FHitResult &outHitResult
){
    if(world == nullptr){
        return false;
    }

    rayScale = std::abs(rayScale);
	FVector End = Start + dir * rayScale; // gx = A + r (B - A)
	bool bHit = world->LineTraceSingleByChannel(outHitResult, Start, End, ECC_Visibility, ignoreParams);
	
	if (bHit){
		outputHit = outHitResult.ImpactPoint; //write impactpoint to output
		return true;
	}

	return false;
}






    
