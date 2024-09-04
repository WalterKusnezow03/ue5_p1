// Fill out your copyright notice in the Description page of Project Settings.


#include "FVectorUtil.h"
#include <cstdlib>


FVectorUtil::FVectorUtil()
{
}

FVectorUtil::~FVectorUtil()
{
}

float FVectorUtil::infinity(){
    return std::numeric_limits<float>::max();
}

/// @brief calculates the normal for a pane Ex: A + r(B-A) + s(C-A)
/// attention, b and c must be positions not directions!
/// @param a ortvektor
/// @param b extend position 1
/// @param c extend position 2
/// @return normale
FVector FVectorUtil::calculateNormal(FVector &a, FVector &b, FVector &c){
    //add normals
    //Ex: A + r(B-A) + s(C-A)
    //n = (B-A) cross (C-A)
    FVector normal = (FVector::CrossProduct((b - a), (c - a))).GetSafeNormal();
    
    //rotate upwards
    FVector center = (a + b + c) / 3;
    FVector applied = center + normal;
    if(applied.Z < center.Z){ //is facing downwards
        normal *= -1;
    }

    //final normal
    return normal;
}

/// @brief calculates the center of 3 given points
/// @param a a in
/// @param b b in
/// @param c c in
/// @return center
FVector FVectorUtil::calculateCenter(FVector &a, FVector &b, FVector &c){
    return ((a+b+c) / 3);
}

/// @brief calculates the center of 4 given points
/// @param a a in
/// @param b b in
/// @param c c in
/// @param d d in
/// @return center
FVector FVectorUtil::calculateCenter(FVector &a, FVector &b, FVector &c, FVector &d){
    return ((a+b+c+d) / 4);
}

/// @brief calcuates the center of any amount of given points
/// @param vec points
/// @return center
FVector FVectorUtil::calculateCenter(std::vector<FVector> &vec){
    FVector sum;
    for (int i = 0; i < vec.size(); i++){
        sum += vec.at(i);
    }
    sum /= vec.size();
    return sum;
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

FVector2D FVectorUtil::randomOffset2D(int range){
    int x = std::rand();
    int y = std::rand();
    x %= range;
    y %= range;
    return FVector2D(
        x,
        y);
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

int FVectorUtil::randomNumber(int lower, int higher){
    int a = randomNumber(higher);
    if(a < lower){
        return lower;
    }
    return a;
}

void FVectorUtil::fillWithRandomNumbers(std::vector<int> &nums, int count, int lower, int higher){
    for (int i = 0; i < count; i++)
    {
        nums.push_back(randomNumber(lower, higher));
    }
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
    //LookAtRotation.Pitch = 0.0f;
    //LookAtRotation.Roll = 0.0f;

    // Apply this rotation to the actor
    return LookAtRotation;
}



/// @brief creates a random rotation
/// @param ownLocation own location of the object
/// @return rotation to set
FRotator FVectorUtil::randomRotation(FVector ownLocation){
    FVector toLook = ownLocation + randomOffset(1000);
    return lookAt(ownLocation, toLook);
}


/// @brief returns a random scale to apply to your aactor
/// @param range range to clamp
/// @param symetric symetric on x,y and z axis or not
/// @return fvector scale
FVector FVectorUtil::randomScale(int lowerRange, int higherRange, bool symetric){

    if(symetric){
        int a = randomNumber(lowerRange, higherRange);
        return FVector(a, a, a);
    }

    int x = randomNumber(lowerRange, higherRange);
    int y = randomNumber(lowerRange, higherRange);
    int z = randomNumber(lowerRange, higherRange);
    return FVector(x, y, z);
}




float FVectorUtil::Dist(FVector2D &a, FVector2D &b){
    FVector2D connect = b - a;
    return connect.Size();
}


/// @brief determines whether a vector between a and b is vertical (points upwards)
/// @param a a in 
/// @param b b in
/// @return facing upwards or not (as whole, order is redundant)
bool FVectorUtil::edgeIsVertical(FVector &A, FVector &B){

    FVector connect = B - A; //AB = B - A
    if(A.Z > B.Z){
        connect = A - B;
    }
    return directionIsVertical(connect);
}

/// @brief will determine if a direction is vertical,
/// MUST be a direction, a connect vector between some point f and g
/// @param A vector direction to check
/// @return vertical or not, facing upwards or not
bool FVectorUtil::directionIsVertical(FVector &A){
    FVector aNormalized = A.GetSafeNormal(); //auf einheitskreis normalisieren
    float skalarProduktUp = std::abs(aNormalized.Z); //Up component
    return skalarProduktUp > 0.7f;  //edge of interest wenn er weit genug nach oben zeigt.
}