// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetInterpolator.h"

TargetInterpolator::TargetInterpolator()
{
}

TargetInterpolator::~TargetInterpolator()
{
}

void TargetInterpolator::useHermiteSplineInterpolation(bool flag){
    vectorInterpolator.setHermiteSplineFlag(flag);
}

bool TargetInterpolator::hasTargetSetup(){
    return 
    vectorInterpolator.TargetSetupFlag() ||
    rotatorInterpolator.TargetSetupFlag();
    
}

void TargetInterpolator::setTarget(FVector fromIn, FVector totarget, float timeToFrameIn){
    vectorInterpolator.setTarget(fromIn, totarget, timeToFrameIn);
}

void TargetInterpolator::setTarget(
	FVector fromIn, 
	FVector toTarget, 
	FRotator fromRotationIn, 
	FRotator toRotationIn, 
	float timeToFrameIn
){
    setTarget(fromIn, toTarget, timeToFrameIn);
    rotatorInterpolator.setTarget(fromRotationIn, toRotationIn, timeToFrameIn);
}

void TargetInterpolator::setTarget(
    FRotator fromRotationIn, 
    FRotator toRotationIn, 
    float timeToFrameIn
){
    rotatorInterpolator.setTarget(fromRotationIn, toRotationIn, timeToFrameIn);
}

/// @brief override target of a RUNNING ANIMATION!
/// @param totarget 
void TargetInterpolator::overrideTarget(FVector totarget){
    vectorInterpolator.overrideTarget(totarget);
}

void TargetInterpolator::overrideStart(FVector fromtarget){
    vectorInterpolator.overrideStart(fromtarget);
}

void TargetInterpolator::overrideStart(FRotator fromRotationIn){
    rotatorInterpolator.overrideStart(fromRotationIn);
}

void TargetInterpolator::overrideStart(FVector fromtarget, FRotator fromRotationIn){
    overrideStart(fromtarget);
    overrideStart(fromRotationIn);
}
void TargetInterpolator::overrideTarget(FVector totarget, FRotator toRotationIn){
    overrideTarget(totarget);
    overrideTarget(toRotationIn);
}

void TargetInterpolator::overrideTarget(FRotator toRotationIn){
    rotatorInterpolator.overrideTarget(toRotationIn);
}

void TargetInterpolator::overrideStartSpeedRelative(FVector newStart, FRotator newRoation){
    vectorInterpolator.overrideStartSpeedRelative(newStart);
    rotatorInterpolator.overrideTarget(newRoation);
    rotatorInterpolator.overrideTime(vectorInterpolator.TimeToFrame());
}


void TargetInterpolator::overrideStartSpeedRelative(FRotator newRotation){
    rotatorInterpolator.overrideStartSpeedRelative(newRotation);
}


void TargetInterpolator::overrideStartSpeedRelative(FVector newStart){
    vectorInterpolator.overrideStartSpeedRelative(newStart);
}








void TargetInterpolator::resetDeltaTime(){
    vectorInterpolator.resetDeltaTime();
    rotatorInterpolator.resetDeltaTime();
}

/// @brief sets the time to frame and reached flag is set to false
/// @param time 
void TargetInterpolator::setNewTimeToFrame(float time){
    vectorInterpolator.overrideTime(time);
    rotatorInterpolator.overrideTime(time);
}


bool TargetInterpolator::hasReachedTarget(){
    return vectorInterpolator.hasReachedTarget() && rotatorInterpolator.hasReachedTarget(); //if not setup = true
}

FVector TargetInterpolator::interpolate(float DeltaTime){
    return vectorInterpolator.interpolate(DeltaTime);
}


FRotator TargetInterpolator::interpolateRotationOnly(float DeltaTime){
    return rotatorInterpolator.interpolate(DeltaTime);
}


FVector TargetInterpolator::interpolate(float DeltaTime, FRotator &rotationOutgoing){
    FVector outpos = interpolate(DeltaTime);
    rotationOutgoing = rotatorInterpolator.interpolate(DeltaTime);
    return outpos;
}

MMatrix TargetInterpolator::interpolateAndGenerateTransform(float DeltaTime){
    MMatrix outMatrix;
    FRotator rotatorOut;
    FVector pos = interpolate(DeltaTime, rotatorOut);
    outMatrix.setRotation(rotatorOut);
    outMatrix.setTranslation(pos);
    return outMatrix;
}

float TargetInterpolator::TimeToFrame(){
    return std::max(vectorInterpolator.TimeToFrame(), rotatorInterpolator.TimeToFrame()); //larger one left.
}

FVector TargetInterpolator::readFromPosition(){
    return vectorInterpolator.readFrom();
}

FVector TargetInterpolator::readToPosition(){
    return vectorInterpolator.readTarget();
}

/** 
 * 
 * 
 * ---- WORLD SPACE SECTION ----
 * 
 * 
 * 
 * man könnte hier noch überlegen ob man start local und worldgleichzeitig baut
 * 
 * 
*/


void TargetInterpolator::overrideStartWorld(FVector targetIn){
    fromWorld = targetIn;
}

void TargetInterpolator::overrideTargetWorld(FVector targetIn){
    targetWorld = targetIn;
    worldtargetSetup = true;
}

/// @brief overrides the starting position of the interpolation in world position
/// if the world target is setup, the local target will be also updated, relative to the actor matrix transform
/// @param newStart new world frame start
/// @param actor actor with rotation and translation
void TargetInterpolator::overrideStartWorldSpeedRelative(FVector newStart, MMatrix &actor){

    MMatrix inverse = actor.jordanInverse();
    if (!worldtargetSetup)
    {
        //translate start to local and override start
        FVector startLocal = inverse * newStart;
        //overrideStartSpeedRelative(startLocal);
        vectorInterpolator.overrideStartSpeedRelative(startLocal);
        return;
    }else{
        
        //translate start and target to local and override both, target must be available
        FVector targetLocalNew = inverse * targetWorld;
        overrideTarget(targetLocalNew);
        FVector startLocal = inverse * newStart;
        //overrideStartSpeedRelative(startLocal);
        vectorInterpolator.overrideStartSpeedRelative(startLocal);
        return;
    }
}


//for door
void TargetInterpolator::resetReachedflag(){
    rotatorInterpolator.resetReachedflag();
    vectorInterpolator.resetReachedflag();
}