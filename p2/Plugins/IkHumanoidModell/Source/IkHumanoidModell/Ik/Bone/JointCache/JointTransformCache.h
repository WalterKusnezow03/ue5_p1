#pragma once


#include "CoreMinimal.h"


class IKHUMANOIDMODELL_API JointTransformCache {

public:
    void UpdateCache(
        FVector &location,
        FRotator &rotation
    ){
        locationCache = location;
        rotationCache = rotation;
    }

    void GetTransform(
        FVector &location,
        FRotator &rotation
    ){
        location = locationCache;
        rotation = rotationCache;
    }

protected:
    FVector locationCache;
    FRotator rotationCache;
};