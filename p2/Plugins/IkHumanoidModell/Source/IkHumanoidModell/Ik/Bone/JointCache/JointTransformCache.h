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

    MMatrix GetTransformAsMatrix(){
        //M = T * R <-- reading direciton -- //t can be overriden, no affection between T * R on multiply
        MMatrix result;
        result.setTranslation(locationCache);
        result.setRotation(rotationCache);
        return result;
    }

protected:
    FVector locationCache;
    FRotator rotationCache;
};