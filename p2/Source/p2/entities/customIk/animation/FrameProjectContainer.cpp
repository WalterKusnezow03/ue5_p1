// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameProjectContainer.h"

FrameProjectContainer::FrameProjectContainer()
{
    world = nullptr;
    velocity = 0.0f;
    maxHeightStartClimb = 99999999.0f;
    minHeightStartClimb = 99999999.0f;
    minHeightStartFalling = -99999999.0f;
}

FrameProjectContainer::~FrameProjectContainer()
{
}


/// @brief saves the data for the next raycast
/// THIS OBJECT IS INTENDED FOR SETUP EACH TIME NEWLY
/// @param worldIn world pointer for raycasting
/// @param currentActorMatrixTemporary actor matrix of starting joint (for example of leg, rotated) for raycasting
/// @param velocity velocity for actor
/// @param lookDir look dir of actor
void FrameProjectContainer::setup(
    UWorld *worldIn, 
    MMatrix &currentActorMatrixTemporary, 
    float velocityIn, 
    FVector lookDirIn,
    float lowerLimitToClimbIn,
    float maxHeightForProjectionIn,
    float minHeightStartFallingIn,
    BoneControllerStates state
){
    updateLocomotionState(state);
    minHeightStartClimb = lowerLimitToClimbIn;
    maxHeightStartClimb = std::abs(maxHeightForProjectionIn);
    minHeightStartFalling = std::abs(minHeightStartFallingIn) * -1.0f;

    if (worldIn != nullptr){
        world = worldIn;
    }
        

    actorMatrixCopy = currentActorMatrixTemporary;
    velocity = velocityIn;
    lookdir = lookDirIn;
}

UWorld *FrameProjectContainer::getWorld(){
    return world;
}

MMatrix &FrameProjectContainer::actorMatrix(){
    return actorMatrixCopy;
}

FVector FrameProjectContainer::getLookDir(){
    return lookdir;
}

float FrameProjectContainer::getVelocity(){
    return velocity;
}



void FrameProjectContainer::updateWorldHitAndOffset(FVector &worldHitIn, FVector &offsetFromOriginalIn){
    worldHit = worldHitIn;
    offsetFromOriginal = offsetFromOriginalIn;
}

FVector FrameProjectContainer::getWorldHit(){
    return worldHit;
}
FVector FrameProjectContainer::getOffsetFromOriginal(){
    return offsetFromOriginal;
}


bool FrameProjectContainer::startClimb(){
    return offsetFromOriginal.Z > minHeightStartClimb;
}

bool FrameProjectContainer::startClimbingAndNoExceedingMaxHeight(){
    return startClimb() && !exceedsMaxHeight();
}

bool FrameProjectContainer::startFalling(){
    return offsetFromOriginal.Z < minHeightStartFalling;
}


/// @brief updates the projection offset and returns whether the maxheight was exceeded
/// @param projectionOffset 
/// @return bool whether max height in respect to offsetFromOriginal was exceeded
bool FrameProjectContainer::exceedsMaxHeight(FVector &projectionOffset){
    offsetFromOriginal = projectionOffset;
    return exceedsMaxHeight();
}

/// @brief updates the projection offset and returns whether the maxheight was exceeded
/// @return bool whether max height in respect to offsetFromOriginal was exceeded
bool FrameProjectContainer::exceedsMaxHeight(){
    return offsetFromOriginal.Z > maxHeightStartClimb; // only positive direction!
}



BoneControllerStates FrameProjectContainer::locomotionState(){
    return currentState;
}
void FrameProjectContainer::updateLocomotionState(BoneControllerStates state){
    currentState = state;
}


bool FrameProjectContainer::locomotionStateIsClimb(){
    return currentState == BoneControllerStates::locomotionClimbAll;
}