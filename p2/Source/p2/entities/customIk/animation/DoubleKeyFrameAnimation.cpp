// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleKeyFrameAnimation.h"

DoubleKeyFrameAnimation::DoubleKeyFrameAnimation()
{
    deltaTime = 0.0f;
}

DoubleKeyFrameAnimation::~DoubleKeyFrameAnimation()
{
}




void DoubleKeyFrameAnimation::setAnimationA(KeyFrameAnimation &&other){
    framesA = MoveTemp(other);
}


bool DoubleKeyFrameAnimation::isAnimationA(){
    return isAnimationAPlaying;
}

bool DoubleKeyFrameAnimation::isAnimationB(){
    return !isAnimationAPlaying;
}



/// @brief gibt den frame zureück der erreicht wurde von der letzen / vorherigen animation (FRAMES A)
/// dazu da um der hip zu sagen wo sie sich relativ befindet und wo sie sich bewegen soll!
/// @return frame reached in animation A, final leg pos made!
FVector DoubleKeyFrameAnimation::readPrevAnimationReachedFrame(){
    return aReachedTickFrame;
}

/// @brief interpolates A or B based on which animation is now wanted
/// @param DeltaTime delta time passed from Tick()
/// @return interpolated frame target
FVector DoubleKeyFrameAnimation::interpolate(float DeltaTime){
    deltaTime += DeltaTime;

    
    FVector interpolated;
    if(isAnimationA()){
        interpolated = interpolateAtarget(DeltaTime);
    }
    else{
        interpolated = interpolateBtarget(DeltaTime);
    }

    //switch wenn zeit überschritten
    if (reachedTime(deltaTime))
    {
        if(!isAnimationAPlaying){
            cycleComplete = true;
            //reset projection offset once animation played trough!
            projectionHipOffset = FVector(0, 0, 0);

        }else{
            //finale position des fusses speichern damit die hip relativ sich weiter bewegen kann
            //relativ zu der erreichten fuss position die ja relativ zum hip hier in der animation ist
            aReachedTickFrame = interpolated;
        }


        isAnimationAPlaying = !isAnimationAPlaying; //flip bool!
        deltaTime = 0.0f;
        currentAndNextOverridenB = false;

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


/// @brief will return if the animation cycle was complete
/// and reset the boolean if true
/// @return animation cycle was complete in the last interpolation (frame)
bool DoubleKeyFrameAnimation::animationCycleWasComplete(){
    if(cycleComplete){
        cycleComplete = false;
        return true;
    }
    return false;
}


/// @brief will return if the time needed is reached to finish an animation or interpolation
/// @param timeCheck 
/// @return 
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


void DoubleKeyFrameAnimation::overrideNextFrameA(FVector &framePos){
    framesA.overrideNextFrame(framePos); // override next target
}


/// @brief updates the interpolation target for interpolation B
/// @param currentNew currentPostion
/// @param nextNew target position
/// @param timeToFrameWanted time to frame
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
    return framesA.readNextFrame();
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
    
    //float timeCopy = framesA.totalLength() + interpolateB.TimeToFrame(); // do in total time (?)
    float timeCopy = interpolateB.TimeToFrame(); //testing needed for only during adjust time!

    float timeInFrames = timeCopy * 60.0f;
    if (timeInFrames > 0.1f){
        projectionHipOffset = offsetMade / timeInFrames; //60fps, nicht einfach teilen, durch zahl der frames!
    }else{
        projectionHipOffset = FVector(0,0,0); //no offset if not possible
    }

    //projektion entfernen wenn man die treppe hochläuft, nur bei negativen werten!
    if(projectionHipOffset.Z > 0.0f){
        projectionHipOffset = FVector(0, 0, 0); //TESTING
    }
}

/// @brief additional hip offset needed because of projected frames, will be (0,0,0) if
/// no offset needed, or any other vector if needed.
/// @return vector offset for hip to add, is timed with animation linear.
FVector DoubleKeyFrameAnimation::getProjectionHipOffsetTimed(){
    /**
     * only return if hip adjust time!
     */
    if(isAnimationB()){
        //DebugHelper::showScreenMessage("HIP TIME!");
        //DebugHelper::showScreenMessage(projectionHipOffset);
        return projectionHipOffset;
    }
    return FVector(0, 0, 0);
    //old
    //return projectionHipOffset;
}




void DoubleKeyFrameAnimation::tryPushFront(FVector &currentLocationRelative){
    framesA.tryPushFront(currentLocationRelative);
}