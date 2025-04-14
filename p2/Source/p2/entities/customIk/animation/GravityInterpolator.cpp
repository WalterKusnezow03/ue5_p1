// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/GravityInterpolator.h"

GravityInterpolator::GravityInterpolator()
{
    resetVelocity();
    reachedFlag = true;
}



GravityInterpolator::~GravityInterpolator()
{
}

/// @brief returns a vector which needs to be added to a position to have gravity based on the
/// passed parameters!
/// @param currentPos 
/// @param DeltaTime 
FVector GravityInterpolator::interpolate(FVector &currentPos, float DeltaTime){
    return interpolate(currentPos, velocityVector, DeltaTime);
}

/// @brief returns a vector which needs to be added to a position to have gravity based on the
/// passed parameters!
/// @param currentPos 
/// @param velocity 
/// @param DeltaTime 
/// @return 
FVector GravityInterpolator::interpolate(FVector &currentPos, FVector &velocity, float DeltaTime){
    if(groundReached(currentPos)){
        DeltaTimeSum = 0.0f;
        resetVelocity();
        return FVector(0,0,0);
    }
    DeltaTimeSum += DeltaTime;

    //x(t) = x0 + v0t + 1/2 at^2
    FVector accelaration = gravityVector();
    FVector accelerationAt2 = 0.5f * accelaration * DeltaTime * DeltaTime;
    FVector interpolatedPos = currentPos + velocity * DeltaTime + accelerationAt2;

    //v(t) = v0 + at
    FVector v0New = (interpolatedPos - currentPos) / DeltaTime;
    //velocity + accelaration * DeltaTime; //hier stimmt noch was nicht
    velocityVector = v0New;

    //AB = B - A
    return interpolatedPos - currentPos;

    //return interpolatedPos;
}



bool GravityInterpolator::groundReached(FVector &compare){
    if(reachedFlag){
        return true;
    }

    //bool distanceReached = FVector::Dist(groundPosition, compare) <= epsilonGroundReached;
    bool belowGround = groundPosition.Z > compare.Z;

    if(belowGround){
        reachedFlag = true;
        return true;
    }
    return false;
}

bool GravityInterpolator::groundReachedFlag(){
    return reachedFlag;
}

void GravityInterpolator::resetVelocity(){
    velocityVector = FVector(0.0f, 0.0f, 0.0f);
}

FVector GravityInterpolator::gravityVector(){
    return FVector(0.0f, 0.0f, gravityA);
}


void GravityInterpolator::updateGroundPosition(FVector &groundIn){
    groundPosition = groundIn;
    reachedFlag = false;
}

void GravityInterpolator::updateGroundPositionAndSetInitialVelocity(
    FVector &groundIn, 
    FVector &initVelocity
){
    updateGroundPosition(groundIn);
    velocityVector = initVelocity;
}

FVector GravityInterpolator::copyGroundPosition(){
    return groundPosition;
}




void GravityInterpolator::setGravity(float gravityin){
    gravityA = std::abs(gravityin) * -1.0f;
}

float GravityInterpolator::gravityCmsDown(){
    return gravityA;
}




float GravityInterpolator::skalar(FVector &currentPos){

    float timeUntilContact = timeUntilGroundContact(currentPos);
    if(std::abs(DeltaTimeSum) >= 0.01f){
        float skalar = timeUntilContact / DeltaTimeSum; // distTarget / distAll
        return skalar;
    }
    return 0.0f;
}

float GravityInterpolator::timeUntilGroundContact(FVector &currentPos){

    float distanceFromGroundMeasured = (currentPos.Z - groundPosition.Z);
    float v0 = velocityVector.Z;
    if (std::abs(v0) <= 0.1f)
    {

        //x(t) = x0 + v0t + 1/2 a t^2
        //gesucht: next time gravity check
        //gesucht also t:

        //x(t) = distanceFromGround + 0*t + 1/2 a * t^2
        //x(t) = distanceFromGround + 1/2 a * t^2
        // 0 = distanceFromGround + 1/2 a * t^2
        // - distanceFromGround = 1/2 a * t^2 | * 2
        // - 2 * distanceFromGround = a * t^2 | :a
        // (-2 * distanceFromGround) / a = t^2 | sqrt
        // sqrt((-2 * distanceFromGround) / a) = t
        // t = sqrt(abs(distance * 2) / abs(gravity))

        float gravity = std::abs(gravityCmsDown());
        if(gravity > 0.01f){

            float nextTime = std::sqrt(
                std::abs(distanceFromGroundMeasured * 2) /
                gravity
            );
            return nextTime;
        }
    }
    else
    {

        //x(t) = distanceFromGround + vt + 1/2 a * t^2
        // 0 = 1/2 a * t^2 + vt + distanceFromGround | * 2
        // 0 = a * t^2 + 2*vt + 2*distanceFromGround | : a
        // 0 = t^2 + (2*vt)/a + (2*distanceFromGround)/a | ax^2 + px + q = 0, a = 1 !

        //(x0, x1) = -(p/2) +- sqrt((p/2)^2-q)

        // p = ((2*v)/a) / 2 <=> ((2*v)/a) * 1/2 <=> v / a
        // (x0, x1) = -(v / a) +- sqrt((v / a)^2 - ((2*distanceFromGround)/a))

        float gravity = std::abs(gravityCmsDown());
        if(gravity > 0.01f){
            float p2 = (v0 / gravity);

            float innerSqrt = (p2 * p2) - 2 * ((distanceFromGroundMeasured) / gravity);
            if (innerSqrt >= 0.0f) {
                float nextTime = std::abs(-1 * p2 + std::sqrt(innerSqrt));
                return nextTime;
            }
        }
    }
    return 0.0f;
}