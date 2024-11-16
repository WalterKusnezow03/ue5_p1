// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleKeyFrameAnimation.h"

DoubleKeyFrameAnimation::DoubleKeyFrameAnimation()
{
    deltaTime = 0.0f;
}

DoubleKeyFrameAnimation::~DoubleKeyFrameAnimation()
{
}


KeyFrameAnimation &DoubleKeyFrameAnimation::currentAnimation(){
    if(isAnimationA()){
        return framesA;
    }else{
        return framesB;
    }
}

KeyFrameAnimation &DoubleKeyFrameAnimation::prevAnimation(){
    if(isAnimationA()){
        return framesB;
    }else{
        return framesA;
    }
}


void DoubleKeyFrameAnimation::setAnimationA(KeyFrameAnimation &&other){
    framesA = MoveTemp(other);
}
void DoubleKeyFrameAnimation::setAnimationB(KeyFrameAnimation &&other){
    framesB = MoveTemp(other);
}


bool DoubleKeyFrameAnimation::isAnimationA(){
    return isAnimationAPlaying;
}

bool DoubleKeyFrameAnimation::isAnimationB(){
    return !isAnimationAPlaying;
}


/// @brief interpolates A or B based on which animation is now wanted
/// @param DeltaTime delta time passed from Tick()
/// @return interpolated frame target
FVector DoubleKeyFrameAnimation::interpolate(float DeltaTime){
    deltaTime += DeltaTime;

    
    FVector interpolated;
    if(isAnimationA()){
        interpolated = interpolateAtarget(DeltaTime);
        aReachedTick = interpolated;
    }
    else{
        interpolated = interpolateBtarget(DeltaTime);
    }

    //switch wenn zeit überschritten
    if (reachedTime(deltaTime))
    {
        isAnimationAPlaying = !isAnimationAPlaying; //flip bool!
        deltaTime = 0.0f;
        currentAndNextOverridenB = false;

        //reset projection offset once animation played trough!
        projectionHipOffset = FVector(0, 0, 0);
    }

    return interpolated;
}


FVector DoubleKeyFrameAnimation::interpolateAtarget(float DeltaTime){
    if(isAnimationA()){
        FVector interpolated = framesA.interpolate(DeltaTime);
        return interpolated;
    }
    return FVector(0, 0, 0);
}

FVector DoubleKeyFrameAnimation::interpolateBtarget(float DeltaTime){
    if(isAnimationB()){
        FVector interpolated = interpolateB.interpolate(DeltaTime);
        return interpolated;
    }
    return FVector(0, 0, 0);
}




bool DoubleKeyFrameAnimation::reachedTime(float timeCheck){
    
    if(isAnimationA()){
        return framesA.reachedLastFrameOfAnimation();
    }
    return timeCheck >= reachTime(); //interpolator B for now 

}

float DoubleKeyFrameAnimation::reachTime(){
    if(isAnimationA()){
        return framesA.totalLength();
    }else{
        return interpolateB.TimeToFrame();
    }
}


void DoubleKeyFrameAnimation::overrideNextFrame(FVector &framePos){
    KeyFrameAnimation &current = currentAnimation();
    return current.overrideNextFrame(framePos); //override next target
}



void DoubleKeyFrameAnimation::tryOverrideCurrentAndNextFrameAnimB(
    FVector &currentNew,
    FVector &nextNew,
    float timeToFrameWanted
){
    if(!currentAndNextOverridenB && isAnimationB()){
        DebugHelper::showScreenMessage("1 override frames! ", FColor::Blue);
        currentAndNextOverridenB = true;
        //framesB.overrideCurrentAndNextFrame(currentNew, nextNew);


        interpolateB.setTarget(currentNew, nextNew, timeToFrameWanted);
    }
}


bool DoubleKeyFrameAnimation::currentAndNextForBOverriden(){
    return currentAndNextOverridenB;
}




/// @brief returns the next frame of animation A (or current anim, but you only need it from A)
/// @return 
FVector DoubleKeyFrameAnimation::readNextFrame(){
    KeyFrameAnimation &current = currentAnimation();
    return current.readNextFrame();
}

/// @brief gibt den frame zureück der erreicht wurde von der letzen / vorherigen animation
/// @return 
FVector DoubleKeyFrameAnimation::readPrevAnimationReachedFrame(){
    return framesA.readLastFrameOfAnimation();
    //return framesA.readPrevFrame();
}

bool DoubleKeyFrameAnimation::nextFrameMustBeGrounded(){
    //KeyFrameAnimation &current = currentAnimation();
    //return current.nextFrameMustBeGrounded();
    return framesA.nextFrameMustBeGrounded();
}

bool DoubleKeyFrameAnimation::nextFrameIsProjected(){
    //KeyFrameAnimation &current = currentAnimation();
    //return current.nextFrameIsProjected();
    return framesA.nextFrameIsProjected();
}






/// @brief will process the offset made from projection for hip adjustment 
///         -> use the getProjectionHipOffsetTimed() method to apply your hip offset over time
/// @param offsetMade 
void DoubleKeyFrameAnimation::processProjectOffset(FVector &offsetMade){
    /**
     * TODO:
     * projection offset sollte jetzt auch anders herum funktionieren (vielleicht?)
     * es wird ja auch r+ckwärts ausgerechnet
     */
    
    float timeCopy = framesA.totalLength();
    float timeToFrames = timeCopy * 60.0f;
    if (timeToFrames != 0.0f){
        projectionHipOffset = offsetMade / timeToFrames; //60fps, nicht einfach teilen, durch zahl der frames!
    }else{
        projectionHipOffset = FVector(0, 0, 0);
    }
}

/// @brief additional hip offset needed because of projected frames, will be (0,0,0) if
/// no offset needed, or any other vector if needed.
/// @return vector offset for hip to add, is timed with animation linear.
FVector DoubleKeyFrameAnimation::getProjectionHipOffsetTimed(){
    return projectionHipOffset;
}
