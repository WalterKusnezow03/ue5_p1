// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/TargetInterpolator.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"

KeyFrameAnimation::KeyFrameAnimation()
{
    frameIndex = 0;
    nextFrameIndex = 1;
    totalLengthSave = 0.0f;
    loop = true;
}

KeyFrameAnimation::KeyFrameAnimation(bool loopIn){
    frameIndex = 0;
    nextFrameIndex = 1;
    totalLengthSave = 0.0f;
    loop = loopIn;
}


KeyFrameAnimation::~KeyFrameAnimation()
{
}





/// @brief add a frame to the animation at a position and distance to previous frame
/// @param position position for frame
/// @param timeFromLastFrame time to keep from last frame in seconds
void KeyFrameAnimation::addFrame(FVector position, float timeFromLastFrame){
    addFrame(position, timeFromLastFrame, false);
}

void KeyFrameAnimation::addFrame(FVector position, float timeFromLastFrame, bool mustBeGrounded){
    totalLengthSave += timeFromLastFrame;
    frames.push_back(KeyFrame(position, timeFromLastFrame, mustBeGrounded));
}

void KeyFrameAnimation::addFrame(
    FVector position,
    float timeFromLastFrame,
    bool mustBeGrounded,
    float clampDistance
){
    FVector zeroVec(0, 0, 0);
    if(FVector::Dist(zeroVec, position) > clampDistance){
        position = position.GetSafeNormal() * clampDistance;
    }

    totalLengthSave += timeFromLastFrame;
    frames.push_back(KeyFrame(position, timeFromLastFrame, mustBeGrounded));
}



/// @brief total length time of the animation
/// @return total length time of the animation
float KeyFrameAnimation::totalLength(){
    return totalLengthSave;
}

/// @brief returns whether the end frame was reached, only is flagged if the animation
/// was not initialized with loop, NOT LOOP!
/// @return reached last frame flag
bool KeyFrameAnimation::reachedLastFrameOfAnimation(){
    if(reachedEndFrameFlag){
        reachedEndFrameFlag = false;
        return true;
    }
    return false;
}

/// @brief returns if enough frames are available for an animation (at least 2)
/// @return has enough or not
bool KeyFrameAnimation::hasAnyFrames(){
    return frames.size() >= 2; //must be at least 2!
}








FVector KeyFrameAnimation::interpolate(float DeltaTime){
    
    if(hasAnyFrames()){
        //DebugHelper::showScreenMessage("nextframe ", nextFrameIndex);
        if (interpolator.hasTargetSetup() == false)
        {
            updateFrameInterpolator();
        }

        FVector interpolated = interpolator.interpolate(DeltaTime);
        //targetCopy = interpolated;

        if(interpolator.hasReachedTarget()){    
            //frameIndex++;
            DebugHelper::showScreenMessage("INTERPOLATOR UPDATE!");
            updateFrameIndex();
            updateFrameInterpolator();
        }

        return interpolated;
    }
    DebugHelper::showScreenMessage("not enough frames!");
    return FVector(0, 0, 0);
}   

void KeyFrameAnimation::updateFrameIndex(){
    frameIndex = (frameIndex + 1) % frames.size();
    nextFrameIndex = (frameIndex + 1) % frames.size();

    bool isEnd = (nextFrameIndex == 0);
    if(isEnd && !loop){
        frameIndex = 0;
        nextFrameIndex = 1;
        
        //set status for asking to end anim
        reachedEndFrameFlag = true;
    }
}

void KeyFrameAnimation::updateFrameInterpolator(){
    KeyFrame &currentFrame = frames.at(frameIndex);
    KeyFrame &nextFrame = frames.at(nextFrameIndex);
    interpolator.setTarget(
        currentFrame.readposition(),
        nextFrame.readposition(),
        nextFrame.timeToFrame()
    );

    //copy next target for the actor to get to adjust hip
    //targetCopy = nextFrame.readposition();
    targetCopy = nextFrame.readposition();

    //reset frame projected status
    frameIsProjected = false;
}

void KeyFrameAnimation::resetIndex(){
    frameIndex = 0;
    nextFrameIndex = 1;
}

bool KeyFrameAnimation::nextFrameMustBeGrounded(){
    if(hasAnyFrames()){
        KeyFrame &nextFrame = frames.at(nextFrameIndex);
        return nextFrame.mustBeGrounded();
    }
    return false;
}

FVector KeyFrameAnimation::readNextFrame(){
    if(hasAnyFrames()){
        KeyFrame &nextFrame = frames.at(nextFrameIndex);
        return nextFrame.readposition();
    }
    return FVector(0, 0, 0);
}

FVector KeyFrameAnimation::readPrevFrame(){
    //KeyFrame &currFrame = frames.at(frameIndex);
    //return currFrame.readposition();
    
    return targetCopy;
}


/// @brief returns frame position of very last key of animation
/// @return 
FVector KeyFrameAnimation::readLastFrameOfAnimation(){
    if(hasAnyFrames()){
        KeyFrame &currFrame = frames.at(frames.size() - 1);
        return currFrame.readposition();
    }
    return targetCopy;
}


/// @brief override the next frame / target value, animation is not overriden, individual to current next
/// frame!
/// @param framePos 
void KeyFrameAnimation::overrideNextFrame(FVector &framePos){
    //DebugHelper::showScreenMessage("override target!");
    interpolator.overrideTarget(framePos);
    targetCopy = framePos; //MUST BE UPDATED TOO!
    frameIsProjected = true;
}

bool KeyFrameAnimation::nextFrameIsProjected(){
    return frameIsProjected;
}





void KeyFrameAnimation::overrideCurrentAndNextFrame(FVector &current, FVector &next){
    interpolator.resetDeltaTime();
    interpolator.overrideStart(current);
    interpolator.overrideTarget(next);
    DebugHelper::showScreenMessage("2 override frames! ", FColor::Blue);
}