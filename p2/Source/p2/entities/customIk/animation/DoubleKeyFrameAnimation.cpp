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

void DoubleKeyFrameAnimation::setAnimationBAdjustPermanentTarget(FVector vector){
    bTarget = vector;
    bIsSetToAutoOverride = true;
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
        interpolated = framesA.interpolate(DeltaTime);

        if(framesA.reachedLastFrameOfAnimation()){
            aReachedTickFrame = interpolated;

            //intepolierte position relativ zur hüfte ist einfach den vektor
            //umzudrehen
            if(bIsSetToAutoOverride){    
                FVector relativeBFrame = aReachedTickFrame * -1; 
                interpolateB.setTarget(relativeBFrame, bTarget, 1.0f);


                
                //nicht das problem.

                DebugHelper::showScreenMessageCompare(
                    "interpolator B TEST : ",
                    relativeBFrame,
                    FVector(-50,0,200), //expected value
                    20
                );
            }
            isAnimationAPlaying = false;
        }


    }
    else{
        interpolated = interpolateB.interpolate(DeltaTime);
        if(interpolateB.hasReachedTarget()){
            cycleComplete = true;
            projectionHipOffset = FVector(0, 0, 0);
            isAnimationAPlaying = true; //flip bool!
            deltaTime = 0.0f;
        }
    }


    return interpolated;
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







/// @brief returns the next frame of animation A (or current anim, but you only need it from A)
/// @return 
FVector DoubleKeyFrameAnimation::readNextFrame(){
    return framesA.readNextFrame();
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
    framesA.tryPushFront(currentLocationRelative, 0.5f); //testing 
}






void DoubleKeyFrameAnimation::projectNextFrameIfNeeded(UWorld *world, MMatrix &actorMatrix){
    if(world != nullptr){
        
        FVector offsetMade(0,0,0);
        bool wasProjected = framesA.projectNextFrameToGroundIfNeeded(world, actorMatrix, offsetMade);
        if(wasProjected){
            //DebugHelper::showScreenMessage("PROCESS OFFSET ", offsetMade, FColor::Orange);
            processProjectOffset(offsetMade);
        }
    }
}