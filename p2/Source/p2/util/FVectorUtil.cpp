// Fill out your copyright notice in the Description page of Project Settings.


#include "FVectorUtil.h"
#include <cstdlib>


FVectorUtil::FVectorUtil()
{
}

FVectorUtil::~FVectorUtil()
{
}


FVector FVectorUtil::randomOffset(int range){

    int x = std::rand();
    int y = std::rand();
    int z = std::rand();

    x %= range;
    y %= range;
    z %= range;

    //return FVector(x, y, z);
     return FVector(
        randomNumber(range),
        randomNumber(range),
        randomNumber(range)
     );
}

int FVectorUtil::randomNumber(int range){
    int r = std::rand();
    r %= range;
    int negate = std::rand() % 2;
    if(negate == 1){
        r *= -1;
    }
    return r;
}

/// @brief finds a look rotation from own location to target
/// @param ownlocation own location
/// @param TargetLocation target to look at
/// @return apply FRotator to actor (setActorRotaion(result))
FRotator FVectorUtil::lookAt(FVector ownlocation, FVector TargetLocation)
{
    // Calculate the rotation needed to look at the target location
    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ownlocation, TargetLocation);

    // Optionally, you can set only the yaw to rotate around the Z-axis
    LookAtRotation.Pitch = 0.0f;
    LookAtRotation.Roll = 0.0f;

    // Apply this rotation to the actor
    return LookAtRotation;
}